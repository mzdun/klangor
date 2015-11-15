#include "TranslationUnit.hh"

#include "Cursor.hh"
#include "DiagnosticSet.hh"
#include "Index.hh"

namespace clang {

TranslationUnit TranslationUnit::create(const IndexView& index, const char *source_filename, int num_clang_command_line_args, const char * const *clang_command_line_args, unsigned num_unsaved_files, CXUnsavedFile *unsaved_files)
{
    return TranslationUnit { clang_createTranslationUnitFromSourceFile(
        index.get(), source_filename, num_clang_command_line_args, clang_command_line_args, num_unsaved_files, unsaved_files
        ) };
}

TranslationUnit TranslationUnit::create(const IndexView& index, const char* ast_filename, CXErrorCode& ec)
{
    CXTranslationUnit tu;
    ec = clang_createTranslationUnit2(index.get(), ast_filename, &tu);
    return TranslationUnit { tu };
}

TranslationUnit TranslationUnit::create(const IndexView& index, const char* ast_filename)
{
    return TranslationUnit { clang_createTranslationUnit(index.get(), ast_filename) };
}

Cursor TranslationUnitView::getCursor()
{
    return { clang_getTranslationUnitCursor(get()) };
}

DiagnosticSet TranslationUnitView::getDiagnosticSet() const
{
    return DiagnosticSet { clang_getDiagnosticSetFromTU(get()) };
}

}
