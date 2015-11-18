#pragma once

#include "Diagnostic.hh"
#include "clang-data.hh"
#include "printer.hh"

#include <cstdio>
#include <vector>

enum class severity {
    note,
    warning,
    error,
    fatal
};

class Report {
    std::unique_ptr<Printer> out;
    std::vector<std::pair<severity, uintmax_t>> stats;
public:
    Report(std::unique_ptr<Printer> out) : out { std::move(out) }
    {
    }

    void report(const diagnostic_location& diag, severity sev, const std::string& msg)
    {
        report(diag, sev, msg.c_str());
    }

    void report(const diagnostic_location& diag, severity sev, const char* msg);
    bool report(const clang::Diagnostic& diag);
};
