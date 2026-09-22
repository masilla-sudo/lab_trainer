#include "trainer/log.hpp"
#include <iostream>

void addEvent(EventLog& log, const std::string& severity,
              const std::string& source, const std::string& message) {
    log.push_back({severity, source, message});
}

void printEventLog(const EventLog& log) {
    if (log.empty()) {
        std::cout << "Журнал пуст.\n";
        return;
    }
    std::cout << "Журнал событий:\n";
    for (const EventRecord& event : log) {
        std::cout << "[" << event.severity << "] "
                  << event.source << ": "
                  << event.message << "\n";
    }
}

void checkParametersAndLogWarnings(const ParameterList& parameters, EventLog& log) {
    for (const ParameterRecord& p : parameters) {
        if (!isInRange(p)) {
            addEvent(log, "WARN", p.name, "значение вне допустимого диапазона");
        }
    }
}