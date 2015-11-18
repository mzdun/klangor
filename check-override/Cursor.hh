#pragma once
#include "clang-cc-base.hh"

namespace clang {

class Cursor {
public:
    using native = CXCursor;

    Cursor();
    Cursor(Cursor&&);
    Cursor& operator=(Cursor&&);
    Cursor(const Cursor&);
    Cursor& operator=(const Cursor&);
    Cursor(const native& pointer);

    const native& get() const
    {
        return pointer;
    }

    explicit operator bool() const
    {
        return !isNull();
    }

    bool isNull() const;
    CXCursorKind getKind() const;
    CXLinkageKind getLinkage() const;
    CXAvailabilityKind getAvailability() const;
    CXLanguageKind getLanguage() const;
    Cursor getSemanticParent() const;
    Cursor getLexicalParent() const;
    CXSourceLocation getLocation() const;
    CXSourceRange getExtent() const;


    template <typename Pred>
    unsigned visitChildren(Pred pred) const
    {
        return clang_visitChildren(get(), cursorVisitor<Pred>, &pred);
    }

private:
    native pointer;

    template <typename Pred>
    static CXChildVisitResult cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
    {
        return (*static_cast<Pred*>(client_data))(Cursor { cursor }, Cursor { parent });
    }
};

}
