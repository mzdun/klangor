#include "Cursor.hh"

namespace clang {

Cursor::Cursor(Cursor&&) = default;
Cursor& Cursor::operator=(Cursor&&) = default;
Cursor::Cursor(const Cursor&) = default;
Cursor& Cursor::operator=(const Cursor&) = default;

Cursor::Cursor()
    : pointer(clang_getNullCursor())
{
}

Cursor::Cursor(const native& pointer) : pointer(pointer)
{
}

bool Cursor::isNull() const
{
    return !!clang_Cursor_isNull(get());
}

CXCursorKind Cursor::getKind() const
{
    return clang_getCursorKind(get());
}

CXLinkageKind Cursor::getLinkage() const
{
    return clang_getCursorLinkage(get());
}

CXAvailabilityKind Cursor::getAvailability() const
{
    return clang_getCursorAvailability(get());
}

CXLanguageKind Cursor::getLanguage() const
{
    return clang_getCursorLanguage(get());
}

Cursor Cursor::getSemanticParent() const
{
    return clang_getCursorSemanticParent(get());
}

Cursor Cursor::getLexicalParent() const
{
    return clang_getCursorLexicalParent(get());
}

CXSourceLocation Cursor::getLocation() const
{
    return clang_getCursorLocation(get());
}

CXSourceRange Cursor::getExtent() const
{
    return clang_getCursorExtent(get());
}

}
