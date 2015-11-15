#pragma once
#include "clang-cc-base.hh"
#include <clang-c\Index.h>

namespace clang {

class DiagnosticSet;
class String;

class DiagnosticView : public View<CXDiagnostic> {
public:
    using View<CXDiagnostic>::View;

    String format(unsigned options = clang_defaultDiagnosticDisplayOptions()) const;
    DiagnosticSet children() const;
    String message() const;
    CXDiagnosticSeverity severity() const;
    CXSourceLocation getLocation() const;
    unsigned getExtents() const;
    CXSourceRange getExtent(unsigned index) const;
};

class Diagnostic : public Handle<Diagnostic, DiagnosticView> {
public:
    using Handle<Diagnostic, DiagnosticView>::Handle;

    static void dispose(CXDiagnostic diag)
    {
        clang_disposeDiagnostic(diag);
    }
};

}
