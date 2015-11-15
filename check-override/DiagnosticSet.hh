#pragma once
#include "clang-cc-base.hh"
#include <clang-c\Index.h>

namespace clang {

class Diagnostic;

class DiagnosticSetView : public View<CXDiagnosticSet> {
public:
    using View<CXDiagnosticSet>::View;

    class Iterator {
        const DiagnosticSetView* owner;
        size_t index;

    public:
        Iterator(const DiagnosticSetView* owner, size_t index)
            : owner(owner)
            , index(index)
        {
        }

        Iterator() = delete;
        Iterator(const Iterator&) = delete;
        Iterator& operator=(const Iterator&) = delete;
        Iterator(Iterator&&) = default;
        Iterator& operator=(Iterator&&) = default;

        bool operator==(const Iterator& rhs) const
        {
            return owner == rhs.owner && index == rhs.index;
        }

        bool operator != (const Iterator& rhs) const
        {
            return !((*this) == rhs);
        }

        Iterator& operator++()
        {
            ++index;
            return *this;
        }

        Diagnostic operator*() const;
    };

    size_t size() const;
    Diagnostic at(size_t index) const;

    Iterator begin() const
    {
        return { this, 0 };
    }
    Iterator end() const
    {
        return { this, size() };
    }
};

class DiagnosticSet : public Handle<DiagnosticSet, DiagnosticSetView> {
public:
    using Handle<DiagnosticSet, DiagnosticSetView>::Handle;

    static void dispose(CXDiagnosticSet index)
    {
        clang_disposeDiagnosticSet(index);
    }
};

}
