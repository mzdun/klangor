#include "String.hh"

namespace clang {
String::String(const CXString& data)
    : data(data)
{
}

String::String()
{
    memset(&data, 0, sizeof(data));
}

String::~String()
{
    clang_disposeString(data);
}

String::String(String&& rhs)
{
    std::swap(data, rhs.data);
}

String& String::operator=(String&& rhs)
{
    std::swap(data, rhs.data);
    return *this;
}

bool String::empty() const
{
    return !c_str();
}

const char* String::c_str() const
{
    return clang_getCString(data);
}


}
