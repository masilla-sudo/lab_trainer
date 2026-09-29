#include "trainer/model.hpp"
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <cmath>
#include <optional>
#include <variant>
#include "trainer/log.hpp"

// --- Вспомогательные функции ---

const std::array<std::string, 5>& visualLayerNames() {
    static const std::array<std::string, 5> layers = {
        "platform_map",
        "passenger_flow",
        "gates",
        "delays",
        "ar_gate_marker"
    };
    return layers;
}

bool isInRange(const ParameterRecord& parameter) {
    return parameter.value >= parameter.minValue &&
           parameter.value <= parameter.maxValue;
}

void printParameters(const ParameterList& parameters) {
    std::cout << "Параметры транспортного узла:\n";
    for (const ParameterRecord& p : parameters) {
        std::cout << "- " << p.name << " = " << p.value << " " << p.unit
                  << " [" << p.minValue << ".." << p.maxValue << "]\n";
    }
}

bool setParameter(ParameterList& parameters, const std::string& name, double newValue) {
    for (ParameterRecord& p : parameters) {
        if (p.name == name) {
            p.value = newValue;
            return true;
        }
    }
    return false;
}

const ParameterRecord* findParameterByName(const ParameterList& parameters,
                                           const std::string& name) {
    auto it = std::find_if(parameters.begin(), parameters.end(),
        [&name](const ParameterRecord& p) {
            return p.name == name;
        });
    return (it != parameters.end()) ? &(*it) : nullptr;
}

std::vector<std::string> getParameterNames(const ParameterList& parameters) {
    std::vector<std::string> names;
    names.reserve(parameters.size());
    std::transform(parameters.begin(), parameters.end(),
                   std::back_inserter(names),
                   [](const ParameterRecord& p) {
                       return p.name;
                   });
    return names;
}

std::vector<std::string> getParameterSummary(const ParameterList& parameters) {
    std::vector<std::string> summary;
    summary.reserve(parameters.size());
    std::transform(parameters.begin(), parameters.end(),
                   std::back_inserter(summary),
                   [](const ParameterRecord& p) {
                       return p.name + " = " + std::to_string(p.value) + " " + p.unit;
                   });
    return summary;
}

std::optional<std::size_t> findParameterIndex(const ParameterList& parameters,
                                             const std::string& name) {
    for (std::size_t i = 0; i < parameters.size(); ++i) {
        if (parameters[i].name == name) {
            return i;
        }
    }
    return std::nullopt;
}

// --- ГЛАВНАЯ ФУНКЦИЯ: applyCommand ---
CommandResult applyCommand(ParameterList& parameters, EventLog& log,
                           const std::string& name, double newValue) {
    
    auto index = findParameterIndex(parameters, name);
    if (!index) {
        addEvent(log, "Error", name, "Параметр не найден: " + name);
        return CommandError{"Параметр не найден: " + name};
    }

    ParameterRecord& p = parameters[*index];
    p.value = newValue;

    double range = p.maxValue - p.minValue;
    double nearThresholdPercent = 0.05;      // 5%
    double criticalThresholdPercent = 0.02; // 2%

    bool isOutOfRange = (p.value < p.minValue || p.value > p.maxValue);
    bool nearMin = false;
    bool nearMax = false;
    bool criticalMin = false;
    bool criticalMax = false;

    if (range > 1e-9) { 
        double nearMinLimit = p.minValue + range * nearThresholdPercent;
        double criticalMinLimit = p.minValue + range * criticalThresholdPercent;
        double nearMaxLimit = p.maxValue - range * nearThresholdPercent;
        double criticalMaxLimit = p.maxValue - range * criticalThresholdPercent;

        nearMin = (p.value >= p.minValue && p.value <= nearMinLimit);
        criticalMin = (p.value >= p.minValue && p.value <= criticalMinLimit);

        nearMax = (p.value >= nearMaxLimit && p.value <= p.maxValue);
        criticalMax = (p.value >= criticalMaxLimit && p.value <= p.maxValue);
    } else {
        // Если диапазон почти нулевой, считаем любое отклонение критическим
        if (std::abs(p.value - p.minValue) > 1e-9) {
            criticalMin = true;
            criticalMax = true;
        }
    }

    if (isOutOfRange) {
        std::string msg = "Значение вне диапазона [" + std::to_string(p.minValue) + ".." +
                          std::to_string(p.maxValue) + "]";
        addEvent(log, "Alarm", name, msg);
        return CommandWarning{msg};
    }

    if (criticalMin || criticalMax) {
        std::string msg = "Внимание: значение " + name + " в критической близости границы диапазона";
        addEvent(log, "Warning", name, msg);
        return CommandWarning{msg};
    }

    if (nearMin || nearMax) {
        std::string msg = "Предупреждение: значение " + name + " близко к границе диапазона";
        addEvent(log, "Warning", name, msg);
        return CommandWarning{msg};
    }

    std::string msg = "Команда принята: " + name + " = " + std::to_string(newValue);
    addEvent(log, "OperatorAction", name, msg);
    return CommandOk{msg};
}

// --- Вспомогательная функция для variant ---
std::string commandResultToText(const CommandResult& result) {
    return std::visit([](const auto& item) {
        return item.text;
    }, result);
}