#pragma once
#include <string>
#include <vector>
#include <array>
#include <cstddef>
#include <optional>
#include <variant>
#include <filesystem>

struct ParameterRecord {
    std::string name;
    double value;
    double minValue;
    double maxValue;
    std::string unit;
};

struct EventRecord {
    std::size_t id{};
    std::string severity;
    std::string source;
    std::string message;
};

using ParameterList = std::vector<ParameterRecord>;
using EventLog = std::vector<EventRecord>;

// --- Variant: результат команды ---
struct CommandOk       { std::string text; };
struct CommandWarning   { std::string text; };
struct CommandError     { std::string text; };
using CommandResult = std::variant<CommandOk, CommandWarning, CommandError>;

// --- Существующие функции ---
const std::array<std::string, 5>& visualLayerNames();
bool isInRange(const ParameterRecord& parameter);
void printParameters(const ParameterList& parameters);
bool setParameter(ParameterList& parameters, const std::string& name, double newValue);

// --- ЛР 7 (STL) ---
std::vector<std::string> getParameterNames(const ParameterList& parameters);
std::vector<std::string> getParameterSummary(const ParameterList& parameters);

// --- ЛР 8: optional — поиск индекса параметра ---
std::optional<std::size_t> findParameterIndex(const ParameterList& parameters,
                                               const std::string& name);

// --- ЛР 8: variant — применение команды с возвратом результата ---
CommandResult applyCommand(ParameterList& parameters, EventLog& log,
                           const std::string& name, double newValue);

// --- ЛР 8: variant → текст через std::visit ---
std::string commandResultToText(const CommandResult& result);