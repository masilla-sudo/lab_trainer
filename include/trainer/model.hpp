#pragma once
#include <string>
#include <vector>
#include <array>
#include <cstddef>

struct ParameterRecord {
    std::string name;
    double value;
    double minValue;
    double maxValue;
    std::string unit;
};

struct EventRecord {
    std::size_t id{};          // номер события (для сортировки)
    std::string severity;      // "Info", "Warning", "Alarm", "OperatorAction"
    std::string source;
    std::string message;
};

using ParameterList = std::vector<ParameterRecord>;
using EventLog = std::vector<EventRecord>;

// --- Существующие функции ---
const std::array<std::string, 5>& visualLayerNames();
bool isInRange(const ParameterRecord& parameter);
void printParameters(const ParameterList& parameters);
bool setParameter(ParameterList& parameters, const std::string& name, double newValue);

// --- Новые функции ЛР 7 (STL-алгоритмы) ---
// find_if: поиск параметра по имени
const ParameterRecord* findParameterByName(const ParameterList& parameters, const std::string& name);
// transform: список имён параметров
std::vector<std::string> getParameterNames(const ParameterList& parameters);
// transform: краткие строки вида "name=value unit"
std::vector<std::string> getParameterSummary(const ParameterList& parameters);