#include "DiagnosticSet.hh"
#include "Diagnostic.hh"

namespace clang {

size_t DiagnosticSetView::size() const
{
    return clang_getNumDiagnosticsInSet(get());
}

Diagnostic DiagnosticSetView::at(size_t index) const
{
    return Diagnostic { clang_getDiagnosticInSet(get(), (unsigned)index) };
}

Diagnostic DiagnosticSetView::Iterator::operator*() const
{
    return owner->at(index);
}

}
