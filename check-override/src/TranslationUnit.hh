#pragma once
#include "clang-cc-base.hh"
#include <clang-c\Index.h>

namespace clang {

class Cursor;
class DiagnosticSet;
class IndexView;

template <>
struct clang_base<CXTranslationUnit> {
    using type = CXTranslationUnitImpl;
};

class TranslationUnitView : public View<CXTranslationUnit> {
public:
    using View<CXTranslationUnit>::View;

    Cursor getCursor();
    DiagnosticSet getDiagnosticSet() const;
};

class TranslationUnit : public Handle<TranslationUnit, TranslationUnitView> {
public:
    using Handle<TranslationUnit, TranslationUnitView>::Handle;

    static void dispose(CXTranslationUnit tu)
    {
        clang_disposeTranslationUnit(tu);
    }

    static TranslationUnit create(const IndexView& index, const char *source_filename, int num_clang_command_line_args, const char * const *clang_command_line_args, unsigned num_unsaved_files, CXUnsavedFile *unsaved_files);
    static TranslationUnit create(const IndexView& index, const char* ast_filename, CXErrorCode& ec);
    static TranslationUnit create(const IndexView& index, const char* ast_filename);

};

}