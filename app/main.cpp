#include <iostream>
#include <string>
#include "trainer/model.hpp"
#include "trainer/log.hpp"
#include "trainer/commands.hpp"

int main() {
    ParameterList parameters = {
        {"passenger_load", 320.0, 0.0, 1000.0, "чел."},
        {"platform_occupancy", 45.0, 0.0, 100.0, "%"},
        {"active_gates", 6.0, 0.0, 8.0, "шт."},
        {"delay_minutes", 12.0, 0.0, 240.0, "мин."}
    };

    EventLog log;
    addEvent(log, "INFO", "system", "тренажер диспетчера транспортного узла запущен");

    std::string line;
    bool running = true;

    while (running) {
        std::cout << "hub-dispatcher> ";
        std::getline(std::cin, line);

        std::vector<std::string> tokens = splitCommand(line);
        if (tokens.empty()) continue;

        const std::string& command = tokens[0];

        if (command == "help") {
            printHelp();
        } else if (command == "status") {
            printParameters(parameters);
        } else if (command == "log") {
            printEventLog(log);
        } else if (command == "visual") {
            printVisualLayers(visualLayerNames());
        } else if (command == "event") {
            if (tokens.size() < 3) {
                std::cout << "Формат: event <severity> <message>\n";
                addEvent(log, "ERROR", "command", "неверный формат event");
                continue;
            }
            std::string sev = tokens[1];
            std::string msg;
            for (size_t i = 2; i < tokens.size(); ++i) {
                if (i > 2) msg += " ";
                msg += tokens[i];
            }
            addEvent(log, sev, "manual", msg);
        } else if (command == "set") {
            if (tokens.size() != 3) {
                std::cout << "Формат: set <parameter> <value>\n";
                addEvent(log, "ERROR", "command", "неверный формат set");
                continue;
            }
            const std::string& name = tokens[1];
            try {
                double value = std::stod(tokens[2]);
                if (setParameter(parameters, name, value)) {
                    addEvent(log, "INFO", name, "значение изменено");
                    checkParametersAndLogWarnings(parameters, log);
                } else {
                    std::cout << "Параметр не найден: " << name << "\n";
                    addEvent(log, "ERROR", name, "параметр не найден");
                }
            } catch (...) {
                std::cout << "Некорректное числовое значение.\n";
                addEvent(log, "ERROR", "command", "ошибка преобразования числа");
            }
        } else if (command == "exit") {
            addEvent(log, "INFO", "system", "тренажер завершён");
            running = false;
        } else {
            std::cout << "Неизвестная команда. Введите help.\n";
            addEvent(log, "ERROR", "command", "неизвестная команда: " + command);
        }
    }
    return 0;
}