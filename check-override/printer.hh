#pragma once

#include <memory>

enum class print {
    source_location,
    note,
    warning,
    error,
    message,
    code,
    supplemental,
    caret,
    caret_tail
};

struct Printer {
    virtual ~Printer() {}
    virtual void out(print type, const char* msg) = 0;
    virtual void endl() = 0;

    static std::unique_ptr<Printer> stdio();
};
