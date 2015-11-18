#pragma once
#include "clang-cc-base.hh"

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
#if CINDEX_VERSION > CINDEX_VERSION_ENCODE(0, 23)
    static TranslationUnit create(const IndexView& index, const char* ast_filename, CXErrorCode& ec);
#endif
    static TranslationUnit create(const IndexView& index, const char* ast_filename);

};

}
