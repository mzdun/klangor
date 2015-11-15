#include "String.hh"

#include "clang-data.hh"

location get_presumed(const CXSourceLocation& loc)
{
    location out;

    clang::String file;
    clang_getPresumedLocation(loc, &file, &out.line, &out.column);
    if (!file.empty())
        out.fname = file.c_str();

    return out;
}

location get_expansion(const CXSourceLocation& loc)
{
    location out;

    CXFile file;
    unsigned offset;
    clang_getExpansionLocation(loc, &file, &out.line, &out.column, &offset);
    if (file) {
        clang::String fname { clang_getFileName(file) };
        out.fname = fname.c_str();
    }

    return out;
}
