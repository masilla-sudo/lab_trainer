#pragma once
#include <string>
#include <vector>
#include <array>

struct ParameterRecord {
    std::string name;
    double value;
    double minValue;
    double maxValue;
    std::string unit;
};

struct EventRecord {
    std::string severity;
    std::string source;
    std::string message;
};

using ParameterList = std::vector<ParameterRecord>;
using EventLog = std::vector<EventRecord>;

const std::array<std::string, 5>& visualLayerNames();
bool isInRange(const ParameterRecord& parameter);
void printParameters(const ParameterList& parameters);
bool setParameter(ParameterList& parameters, const std::string& name, double newValue);