#pragma once

#include "Cursor.hh"
#include "clang-data.hh"

bool has_override_attr(const clang::Cursor& cursor);
symbol get_method(CXTranslationUnit tu, const clang::Cursor& cursor);
