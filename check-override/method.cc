#include "method.hh"

#include "String.hh"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

bool has_override_attr(const clang::Cursor& cursor)
{
    bool ret = false;
    cursor.visitChildren([&](const clang::Cursor& cursor, const clang::Cursor&) {
        if (cursor.getKind() == CXCursor_CXXOverrideAttr || cursor.getKind() == CXCursor_CXXFinalAttr) {
            ret = true;
            return CXChildVisit_Break;
        }
        return CXChildVisit_Continue;
    });

    return ret;
}

symbol get_method(CXTranslationUnit tu, const clang::Cursor& cursor)
{
    symbol out;

    auto loc = cursor.getLocation();
    out.loc = get_presumed(loc);

    auto actual = get_expansion(loc);
    out.path = actual.fname;

    auto range = clang_getRange(loc, loc);
    CXToken *tokens = nullptr;
    unsigned count = 0;
    clang_tokenize(tu, range, &tokens, &count);

    auto deleter = [=](CXToken * tokens) { clang_disposeTokens(tu, tokens, count);  };
    std::unique_ptr<CXToken, decltype(deleter)> anchor { tokens, deleter };

    auto crsrs = std::make_unique<CXCursor[]>(count);
    clang_annotateTokens(tu, tokens, count, crsrs.get());

    for (unsigned i = 0; i < count; ++i) {
        auto& tok = tokens[i];

        auto ref_kind = clang_getCursorKind(crsrs[i]);
        if (ref_kind != CXCursor_CXXMethod)
            continue;

        auto kind = clang_getTokenKind(tok);
        if (kind != CXToken_Identifier)
            continue;

        clang::String spelling { clang_getTokenSpelling(tu, tok) };
        out.name = spelling.c_str();

        auto extent = clang_getTokenExtent(tu, tok);
        out.extent.start = get_presumed(clang_getRangeStart(extent));
        out.extent.end = get_presumed(clang_getRangeEnd(extent));
        break;
    }

    return out;
}
