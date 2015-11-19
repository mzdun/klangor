#include "report.hh"

#include "DiagnosticSet.hh"
#include "String.hh"

#include <fstream>
#include <string>
#include <tuple>

#ifdef _MSC_VER
#define MSC_LINES
#endif

namespace {
std::string code(const diagnostic_location& diag)
{
    std::string line;

    std::ifstream i { diag.path, std::ios::in | std::ios::binary };
    if (!i.is_open())
        return line;

    auto count = diag.extent.start.line;
    while (count--) {
        std::getline(i, line);
    }
    return line;
}

std::tuple<std::string, std::string, bool> build_carret(const diagnostic_location& diag, const std::string& s)
{
    std::tuple<std::string, std::string, bool> out;

    auto start = diag.extent.start.column;
    auto end = diag.extent.end.column;
    if (end <= start) {
        if (!end || !start) {
            std::get<2>(out) = false;
            return out;
        }
        end = start + 1;
    }
    std::get<2>(out) = true;

    auto c = s.begin();
    auto e = s.end();

    if (start) {
        auto counter = start - 1;
        std::get<0>(out).reserve(counter);
        while (c != e && counter) {
            std::get<0>(out).push_back(*c == '\t' ? '\t' : ' ');
            --counter;
            ++c;
        }
    }

    std::get<1>(out).assign(end - start - 1, '~');

    return out;
}
}

void Report::report(const diagnostic_location& diag, severity sev, const char* msg)
{
    if (!diag.loc.fname.empty()) {
#if defined(MSC_LINES)
        auto loc = diag.loc.fname + "(" + std::to_string(diag.loc.line) + "): ";
#else
        auto loc = diag.loc.fname + ":" + std::to_string(diag.loc.line) + ":" + std::to_string(diag.loc.column) + ": ";
#endif
        out->out(print::source_location, loc.c_str());
    }

    switch (sev) {
    case severity::note: out->out(print::note, "note: "); break;
    case severity::warning: out->out(print::warning, "warning: "); break;
    case severity::error: out->out(print::error, "error: "); break;
    case severity::fatal: out->out(print::error, "fatal error: "); break;
    }

    out->out(print::message, msg);
    out->endl();

    if (diag.loc.fname.empty())
        return;

    // test loc.fname == actual; (absolute/canonical) - if not, actual symbol might be somewhere else...

    auto line = code(diag);
    out->out(print::code, line.c_str());
    out->endl();

    std::string prefix, tail;
    bool present = false;
    std::tie(prefix, tail, present) = build_carret(diag, line);
    if (present) {
        out->out(print::supplemental, prefix.c_str());
        out->out(print::caret, "^");
        out->out(print::caret_tail, tail.c_str());
        out->endl();
    }
}

bool Report::report(const clang::Diagnostic& diag)
{
    severity sev = severity::note;
    switch (diag.severity()) {
    case CXDiagnostic_Ignored: return true;
    case CXDiagnostic_Note: sev = severity::note; break;
    case CXDiagnostic_Warning: sev = severity::warning; break;
    case CXDiagnostic_Error: sev = severity::error; break;
    case CXDiagnostic_Fatal: sev = severity::fatal; break;
    };

    diagnostic_location local;

    local.loc = get_presumed(diag.getLocation());
    auto actual = get_expansion(diag.getLocation());
    local.path = actual.fname;

    auto ranges = diag.getExtents();
    if (ranges) {
        auto extent = diag.getExtent(0);
        local.extent.start = get_presumed(clang_getRangeStart(extent));
        local.extent.end = get_presumed(clang_getRangeEnd(extent));
    } else {
        local.extent.start = local.loc;
        local.extent.end = local.loc;
        ++local.extent.end.column;
    }

    auto str = diag.message();
    report(local, sev, str.c_str());

    for (auto child : diag.children()) {
        if (!report(child))
            return false;
    }

    return sev != severity::fatal;
}
