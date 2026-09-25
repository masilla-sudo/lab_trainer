#pragma once
#include <string>

enum class Severity {
    Ok,
    Warning,
    Critical
};

struct CheckResult {
    std::string parameterId;
    std::string message;
    Severity severity = Severity::Ok;
};