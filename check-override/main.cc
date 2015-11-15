#include "Cursor.hh"
#include "Index.hh"
#include "TranslationUnit.hh"
#include "Diagnostic.hh"
#include "DiagnosticSet.hh"

#include "clang-data.hh"
#include "method.hh"
#include "report.hh"

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

void visit_children(CXTranslationUnit tu, const clang::Cursor& cursor, Report& output, int indent);

void visit_method(CXTranslationUnit tu, const clang::Cursor& cursor, Report& output);

auto make_visitor(CXTranslationUnit tu, Report& output, int indent)
{
    return [=, &output](const clang::Cursor& cursor, const clang::Cursor&) {
#ifdef PRINT_TREE
        if (indent)
            std::fprintf(stderr, "%*c", indent, ' ');
        std::fprintf(stderr, "%s\n", cursor_kind(cursor.getKind()));
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

int main(int argc, char* argv[])
{
    auto index = clang::Index::create(0, 0);
    CXErrorCode ec;
    auto tu = index.parse(nullptr, argv, argc, nullptr, 0, CXTranslationUnit_SkipFunctionBodies, ec);

    Report output { stderr };

    bool has_errors = 0;
    for (auto diag : tu.getDiagnosticSet()) {
        if (diag.severity() > CXDiagnostic_Warning) {
            if (!output.report(diag)) // fatal
                return 1;
            has_errors = true;
        }
    }

    auto visitor = make_visitor(tu.get(), output, 0);
    visitor(tu.getCursor(), clang::Cursor { });

    return 0;
}
