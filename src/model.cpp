#include "trainer/model.hpp"
#include <iostream>

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