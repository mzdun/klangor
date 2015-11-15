#include "report.hh"

#include "DiagnosticSet.hh"
#include "String.hh"

#include <fstream>
#include <string>
#include <tuple>

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

std::tuple<std::string, std::string> build_carret(const diagnostic_location& diag, const std::string& s)
{
    std::tuple<std::string, std::string> out;

    auto c = s.begin();
    auto e = s.end();

    auto counter = diag.extent.start.column - 1;
    std::get<0>(out).reserve(counter);
    while (c != e && counter) {
        std::get<0>(out).push_back(*c == '\t' ? '\t' : ' ');
        --counter;
        ++c;
    }

    counter = diag.extent.end.column - diag.extent.start.column - 1;
    std::get<1>(out).assign(counter, '~');

    return out;
}
}

void Report::report(const diagnostic_location& diag, severity sev, const char* msg)
{
    if (!diag.loc.fname.empty()) {
#if defined(MSC_LINES)
        std::fprintf(out, "%s(%u): ", diag.loc.fname.c_str(), diag.loc.line);
#else
        std::fprintf(out, "%s:%u:%u: ", diag.loc.fname.c_str(), diag.loc.line, diag.loc.column);
#endif
    }

    switch (sev) {
    case severity::note: std::fprintf(out, "note: "); break;
    case severity::warning: std::fprintf(out, "warning: "); break;
    case severity::error: std::fprintf(out, "error: "); break;
    case severity::fatal: std::fprintf(out, "fatal error: "); break;
    }

    std::fputs(msg, out);
    std::fputc('\n', out);

    if (diag.loc.fname.empty())
        return;

    // test loc.fname == actual; (absolute/canonical) - if not, actual symbol might be somewhere else...

    auto line = code(diag);
    std::fprintf(stdout, "%s\n", line.c_str());
    std::string prefix, tail;
    std::tie(prefix, tail) = build_carret(diag, line);
    std::fprintf(stdout, "%s^%s\n", prefix.c_str(), tail.c_str());
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
