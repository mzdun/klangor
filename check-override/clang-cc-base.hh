#pragma once

#include <memory>

namespace clang {

template <typename Native>
class View {
public:
    using native = Native;
    native get() const { return pointer; }
    explicit operator bool() const
    {
        return pointer;
    }

protected:
    View() : pointer { }
    {
    }

    explicit View(native pointer) : pointer { pointer }
    {
    }
private:
    native pointer;
};

template <typename CXPointer>
struct clang_base {
    using type = void;
};
template <typename CXPointer>
using clang_base_t = typename clang_base<CXPointer>::type;

template <typename Final, typename ViewT>
class Handle : public ViewT
{
    using clang_t = typename ViewT::native;
    using clang_base_t = clang_base_t<clang_t>;

    struct Deleter {

        void operator()(clang_t value)
        {
            Final::dispose(value);
        }
    };

    class unique_ptr : public std::unique_ptr<clang_base_t, Deleter> {
    public:
        using unique = std::unique_ptr<clang_base_t, Deleter>;
        using unique::unique;
        clang_t get() const
        {
            return unique::get();
        }
    };

    unique_ptr handle;
public:
    Handle() = default;
    Handle(Handle&&) = default;
    Handle& operator=(Handle&&) = default;
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    explicit Handle(typename ViewT::native handle) : ViewT { handle }, handle { handle }
    {
    }
};


}
