#pragma once

#include <clang-c\Index.h>
#include <string>


struct position {
    unsigned line = 0;
    unsigned column = 0;
};

struct location : position {
    std::string fname;
};

struct range {
    position start, end;
};

struct diagnostic_location {
    location loc;
    range extent;
    std::string path;
};

struct symbol : diagnostic_location {
    std::string name;
};

location get_presumed(const CXSourceLocation& loc);
location get_expansion(const CXSourceLocation& loc);
