#pragma once
#include "dispatcher/core/check_result.hpp"
#include "dispatcher/core/parameter.hpp"
#include "dispatcher/core/concepts.hpp"
#include <sstream>
#include <string>

struct VisualDescriptor {
    std::string objectId;
    std::string visualKind;
    std::string label;
    std::string color;
    std::string valueStr;
    int priority = 0;
};

inline std::string colorBySeverity(Severity severity) {
    switch (severity) {
        case Severity::Ok:       return "green";
        case Severity::Warning:  return "yellow";
        case Severity::Critical: return "red";
        default:                 return "blue";
    }
}

template <Streamable T>
VisualDescriptor makeDescriptor(const Parameter<T>& param, const CheckResult& result) {
    VisualDescriptor desc;
    desc.objectId = param.id();
    desc.label = param.label();
    desc.color = colorBySeverity(result.severity);

    std::ostringstream oss;
    oss << param.value();
    desc.valueStr = oss.str();

    if (result.severity == Severity::Critical) {
        desc.visualKind = "ar_marker";
        desc.priority = 100;
    } else if (result.severity == Severity::Warning) {
        desc.visualKind = "badge";
        desc.priority = 50;
    } else {
        desc.visualKind = "gauge";
        desc.priority = 10;
    }

    return desc;
}