#include "Diagnostic.hh"

#include "DiagnosticSet.hh"
#include "String.hh"

namespace clang {

String DiagnosticView::format(unsigned options) const
{
    return { clang_formatDiagnostic(get(), options) };
}

DiagnosticSet DiagnosticView::children() const
{
    return DiagnosticSet { clang_getChildDiagnostics(get()) };
}

String DiagnosticView::message() const
{
    return clang_getDiagnosticSpelling(get());
}

CXDiagnosticSeverity DiagnosticView::severity() const
{
    return clang_getDiagnosticSeverity(get());
}

CXSourceLocation DiagnosticView::getLocation() const
{
    return clang_getDiagnosticLocation(get());
}

unsigned DiagnosticView::getExtents() const
{
    return clang_getDiagnosticNumRanges(get());
}

CXSourceRange DiagnosticView::getExtent(unsigned index) const
{
    return clang_getDiagnosticRange(get(), index);
}

}
