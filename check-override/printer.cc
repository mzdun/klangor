#include "printer.hh"

#if defined _WIN32
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>

struct StdIOPrinter : public Printer {
    StdIOPrinter() {}
    void out(print type, const char* msg) override;
    void endl() override;
};

std::unique_ptr<Printer> Printer::stdio()
{
    return std::make_unique<StdIOPrinter>();
}

#if defined _WIN32
inline bool is_a_tty(int fd)
{
    return !!_isatty(fd);
}

inline int file_no(FILE* file)
{
    return _fileno(file);
}

class ColorSetup {
    const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    const WORD old_color_attrs = CurrentColor();
    bool use_color;

    WORD CurrentColor()
    {
        CONSOLE_SCREEN_BUFFER_INFO buffer_info;
        GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
        return buffer_info.wAttributes;
    }
public:
    ColorSetup(bool use_color, print type) : use_color { use_color }
    {
        if (!use_color)
            return;

        WORD color = 0;
        switch (type) {
        case print::source_location:
            color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case print::warning:
            color = FOREGROUND_RED | FOREGROUND_BLUE; break;
        case print::error:
            color = FOREGROUND_RED; break;
        case print::caret:
        case print::caret_tail:
            color = FOREGROUND_GREEN; break;

        case print::note:
        case print::message:
        case print::code:
        case print::supplemental:
            this->use_color = false;
            return;
        };

        std::fflush(stdout);
        SetConsoleTextAttribute(stdout_handle, color);
    }
    ~ColorSetup()
    {
        if (!use_color)
            return;

        std::fflush(stdout);
        SetConsoleTextAttribute(stdout_handle, old_color_attrs);
    }
};
#else
inline bool is_a_tty(int fd)
{
    return !!isatty(fd);
}

inline int file_no(FILE* file)
{
    return fileno(file);
}

class ColorSetup {
    bool use_color;
public:
    ColorSetup(bool color, print type) : use_color { color }
    {
        if (!use_color)
            return;

        int grp = 0;
        int param = -1;
        switch (type) {
        case print::source_location:
            param = 97; break;
        case print::note:
            param = 90; break;
        case print::warning:
            param = 95; break;
        case print::error:
            param = 91; break;
        case print::caret:
            param = 92; break;
        case print::caret_tail:
            grp = 1;
            param = 32; break;

        case print::message:
        case print::code:
        case print::supplemental:
            use_color = false;
            return;
        };

        std::printf("\033[%d;%dm", grp, param);
    }
    ~ColorSetup()
    {
        if (!use_color)
            return;

        std::printf("\033[m");
    }
};
#endif

bool should_use(bool stdout_is_tty)
{
#if defined _WIN32
    return stdout_is_tty;
#else
    if (!stdout_is_tty)
        return false;
    const char* const term = std::getenv("TERM");
    return
        (!std::strcmp(term, "xterm")) ||
        (!std::strcmp(term, "xterm-color")) ||
        (!std::strcmp(term, "xterm-256color")) ||
        (!std::strcmp(term, "screen")) ||
        (!std::strcmp(term, "screen-256color")) ||
        (!std::strcmp(term, "linux")) ||
        (!std::strcmp(term, "cygwin"));
#endif
}

void StdIOPrinter::out(print type, const char* msg)
{
    ColorSetup color { should_use(is_a_tty(file_no(stdout))), type };
    std::fputs(msg, stdout);
}

void StdIOPrinter::endl()
{
    std::putchar('\n');
}
