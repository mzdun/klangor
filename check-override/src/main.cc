#include "Cursor.hh"
#include "Index.hh"
#include "TranslationUnit.hh"
#include "Diagnostic.hh"
#include "DiagnosticSet.hh"
#include "String.hh"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#define MSC_LINES
// #define PRINT_TREE

#ifdef PRINT_TREE

#include "cursor_type_x.inl"
const char* cursor_kind(CXCursorKind kind)
{
#define CURSOR_NAME(name) case CXCursor_ ## name: return #name;
    switch (kind) {
        CURSOR_KIND(CURSOR_NAME)
    };
#undef CURSOR_NAME

    return "<unknown>";
}

#endif // PRINT_TREE


struct position {
    unsigned line = 0;
    unsigned column = 0;
};

struct location : position {
    std::string fname;
};

struct range {
    position start, end;
};

struct diagnostic_location {
    location loc;
    range extent;
    std::string path;
};

struct symbol : diagnostic_location {
    std::string name;
};

location get_presumed(const CXSourceLocation& loc)
{
    location out;

    clang::String file;
    clang_getPresumedLocation(loc, &file, &out.line, &out.column);
    if (!file.empty())
        out.fname = file.c_str();

    return out;
}

location get_expansion(const CXSourceLocation& loc)
{
    location out;

    CXFile file;
    unsigned offset;
    clang_getExpansionLocation(loc, &file, &out.line, &out.column, &offset);
    if (file) {
        clang::String fname { clang_getFileName(file) };
        out.fname = fname.c_str();
    }

    return out;
}

bool has_override_attr(const clang::Cursor& cursor)
{
    bool ret = false;
    cursor.visitChildren([&](const clang::Cursor& cursor, const clang::Cursor&) {
        if (cursor.getKind() == CXCursor_CXXOverrideAttr) {
            ret = true;
            return CXChildVisit_Break;
        }
        return CXChildVisit_Continue;
    });

    return ret;
}

symbol get_method(CXTranslationUnit tu, const clang::Cursor& cursor)
{
    symbol out;

    auto loc = cursor.getLocation();
    out.loc = get_presumed(loc);

    auto actual = get_expansion(loc);
    out.path = actual.fname;

    auto range = clang_getRange(loc, loc);
    CXToken *tokens = nullptr;
    unsigned count = 0;
    clang_tokenize(tu, range, &tokens, &count);

    auto deleter = [=](CXToken * tokens) { clang_disposeTokens(tu, tokens, count);  };
    std::unique_ptr<CXToken, decltype(deleter)> anchor { tokens, deleter };

    auto crsrs = std::make_unique<CXCursor[]>(count);
    clang_annotateTokens(tu, tokens, count, crsrs.get());

    for (unsigned i = 0; i < count; ++i) {
        auto& tok = tokens[i];

        auto ref_kind = clang_getCursorKind(crsrs[i]);
        if (ref_kind != CXCursor_CXXMethod)
            continue;

        auto kind = clang_getTokenKind(tok);
        if (kind != CXToken_Identifier)
            continue;

        clang::String spelling { clang_getTokenSpelling(tu, tok) };
        out.name = spelling.c_str();

        auto extent = clang_getTokenExtent(tu, tok);
        out.extent.start = get_presumed(clang_getRangeStart(extent));
        out.extent.end = get_presumed(clang_getRangeEnd(extent));
        break;
    }

    return out;
}

enum class severity {
    note,
    warning,
    error,
    fatal
};

class Report {
    FILE* out;
    std::vector<std::pair<severity, uintmax_t>> stats;

    static std::string code(const diagnostic_location& diag)
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

    static std::tuple<std::string, std::string> build_carret(const diagnostic_location& diag, const std::string& s)
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

public:
    Report(FILE* out) : out { out }
    {
    }

    void report(const diagnostic_location& diag, severity sev, const std::string& msg)
    {
        report(diag, sev, msg.c_str());
    }

    void report(const diagnostic_location& diag, severity sev, const char* msg)
    {
        if (!diag.loc.fname.empty()) {
#if defined(MSC_LINES)
            fprintf(out, "%s(%u): ", diag.loc.fname.c_str(), diag.loc.line);
#else
            fprintf(out, "%s:%u:%u: ", diag.loc.fname.c_str(), diag.loc.line, diag.loc.column);
#endif
        }

        switch (sev) {
        case severity::note: fprintf(out, "note: "); break;
        case severity::warning: fprintf(out, "warning: "); break;
        case severity::error: fprintf(out, "error: "); break;
        case severity::fatal: fprintf(out, "fatal error: "); break;
        }

        fputs(msg, out);
        fputc('\n', out);

        if (diag.loc.fname.empty())
            return;

        // test loc.fname == actual; (absolute/canonical) - if not, actual symbol might be somewhere else...

        auto line = code(diag);
        fprintf(stdout, "%s\n", line.c_str());
        std::string prefix, tail;
        std::tie(prefix, tail) = build_carret(diag, line);
        fprintf(stdout, "%s^%s\n", prefix.c_str(), tail.c_str());
    }

};

void visit_children(CXTranslationUnit tu, const clang::Cursor& cursor, Report& output, int indent);

void visit_method(CXTranslationUnit tu, const clang::Cursor& cursor, Report& output);

auto make_visitor(CXTranslationUnit tu, Report& output, int indent)
{
    return [=, &output](const clang::Cursor& cursor, const clang::Cursor&) {
#ifdef PRINT_TREE
        if (indent)
            printf("%*c", indent, ' ');
        printf("%s\n", cursor_kind(cursor.getKind()));
#endif // PRINT_TREE

        if (cursor.getKind() == CXCursor_CXXMethod) {
            // visit_children(tu, cursor, indent + 4);
            visit_method(tu, cursor, output);
            return CXChildVisit_Continue;
        }

        visit_children(tu, cursor, output, indent + 4);
        return CXChildVisit_Continue;
    };
}

void visit_method(CXTranslationUnit tu, const clang::Cursor& cursor, Report& output)
{
    if (!clang_CXXMethod_isVirtual(cursor.get()))
        return;

    if (clang_Location_isInSystemHeader(cursor.getLocation()))
        return;

    if (has_override_attr(cursor))
       return;

    auto info = get_method(tu, cursor);

    if (info.name.empty())
        return;

    CXCursor* overriden = nullptr;
    unsigned count = 0;
    clang_getOverriddenCursors(cursor.get(), &overriden, &count);
    std::unique_ptr<CXCursor, decltype(&clang_disposeOverriddenCursors)> anchor { overriden, clang_disposeOverriddenCursors };

    if (!count)
        return;

    output.report(info, severity::warning, "'" + info.name + "' overrides a member function but is not marked 'override'");

    for (unsigned i = 0; i < count; ++i) {
        clang::Cursor cur { overriden[i] };

        auto ref = get_method(tu, cur);

        output.report(ref, severity::note, "overridden virtual function is here");
    }
}

void visit_children(CXTranslationUnit tu, const clang::Cursor& cursor, Report& output, int indent)
{
    cursor.visitChildren(make_visitor(tu, output, indent));
}

bool print(const clang::Diagnostic& diag, Report& output)
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
    output.report(local, sev, str.c_str());

    for (auto child : diag.children()) {
        if (!print(child, output))
            return false;
    }

    return sev != severity::fatal;
}

int main(int argc, char* argv[])
{
    auto index = clang::Index::create(0, 0);
    CXErrorCode ec;
    auto tu = index.parse(nullptr, argv, argc, nullptr, 0, CXTranslationUnit_SkipFunctionBodies, ec);

    Report output { stderr };

    bool has_errors = 0;
    for (auto diag : tu.getDiagnosticSet()) {
        if (diag.severity() > CXDiagnostic_Warning) {
            if (!print(diag, output)) // fatal
                return 1;
            has_errors = true;
        }
    }

    auto visitor = make_visitor(tu.get(), output, 0);
    visitor(tu.getCursor(), clang::Cursor { });

    return 0;
}
