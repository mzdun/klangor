#pragma once
#include "clang-cc-base.hh"

namespace clang {
class String {
    CXString data;

public:
    String(const CXString& data);
    String();
    ~String();
    String(String&&);
    String& operator=(String&&);

    String(const String&) = delete;
    String& operator=(const String&) = delete;

    bool empty() const;
    const char* c_str() const;

    CXString* operator&() { return &data; }
    const CXString* operator&() const { return &data; }
};

}
