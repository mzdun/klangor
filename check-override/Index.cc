#include "Index.hh"
#include "TranslationUnit.hh"

namespace clang {

Index Index::create(int excludeDeclarationsFromPCH, int displayDiagnostics)
{
    return Index { clang_createIndex(excludeDeclarationsFromPCH, displayDiagnostics) };
}

void IndexView::setGlobalOptions(unsigned options) const
{
    return clang_CXIndex_setGlobalOptions(get(), options);
}

unsigned IndexView::getGlobalOptions() const
{
    return clang_CXIndex_getGlobalOptions(get());
}

TranslationUnit IndexView::parse(const char *source_filename, const char *const *command_line_args, int num_command_line_args, CXUnsavedFile *unsaved_files, unsigned num_unsaved_files, unsigned options) const
{
    return TranslationUnit { clang_parseTranslationUnit(get(), source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, options) };
}

TranslationUnit IndexView::parse(const char *source_filename, const char *const *command_line_args, int num_command_line_args, CXUnsavedFile *unsaved_files, unsigned num_unsaved_files, unsigned options, CXErrorCode& ec) const
{
    CXTranslationUnit tu;
    ec = clang_parseTranslationUnit2(get(), source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, options, &tu);
    return TranslationUnit { tu };
}

}
