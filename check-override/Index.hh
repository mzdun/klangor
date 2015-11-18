#pragma once
#include "clang-cc-base.hh"

namespace clang {

class TranslationUnit;

class IndexView : public View<CXIndex> {
public:
    using View<CXIndex>::View;

    void setGlobalOptions(unsigned options) const;
    unsigned getGlobalOptions() const;

#if CINDEX_VERSION > CINDEX_VERSION_ENCODE(0, 23)
    TranslationUnit parse(const char *source_filename, const char *const *command_line_args, int num_command_line_args, CXUnsavedFile *unsaved_files, unsigned num_unsaved_files, unsigned options, CXErrorCode&) const;
#endif
    TranslationUnit parse(const char *source_filename, const char *const *command_line_args, int num_command_line_args, CXUnsavedFile *unsaved_files, unsigned num_unsaved_files, unsigned options) const;

    // CXIndexAction IndexAction_create(CXIndex CIdx);
};

class Index : public Handle<Index, IndexView> {
public:
    using Handle<Index, IndexView>::Handle;

    static void dispose(CXIndex index)
    {
        clang_disposeIndex(index);
    }

    static Index create(int excludeDeclarationsFromPCH, int displayDiagnostics);
};

}
