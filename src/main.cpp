#include "trainer/model.hpp"
#include "trainer/log.hpp"
#include "trainer/commands.hpp"

#include <iostream>
#include <string>
#include <cassert>
#include <filesystem>

int main() {
    // --- Исходные параметры диспетчерского тренажёра (вариант Е) ---
    ParameterList parameters = {
        {"V-101", 55.0,   0.0,  80.0,  "км/ч"},
        {"I-201",  7.0,   5.0,  10.0,  "мин"},
        {"O-301",  2.0,   0.0,   5.0,  "пути"},
        {"C-401",  1.0,   0.0,   3.0,  "канал"},
    };

    // --- Исходный журнал событий ---
    EventLog log;
    addEvent(log, "Info",           "system",  "Тренажёр запущен");
    addEvent(log, "OperatorAction", "V-101",   "Оператор изменил скорость");
    addEvent(log, "Warning",        "I-201",   "Интервал ниже нормы");
    addEvent(log, "Alarm",          "O-301",   "Занятость пути превышена");
    addEvent(log, "Info",           "system",  "Проверка параметров завершена");

    std::cout << "=== Диспетчерский тренажёр (ЛР 8: modern C++ — optional, variant, filesystem) ===\n\n";

    // --- Самопроверка (assert) ---
    assert(findParameterIndex(parameters, "V-101").has_value());
    assert(!findParameterIndex(parameters, "UNKNOWN").has_value());

    // Проверка variant-команды
    auto testResult = applyCommand(parameters, log, "V-101", 60.0);
    std::cout << "[Тест] " << commandResultToText(testResult) << "\n\n";

    // --- Тесты граничных значений для V-101 ---
    // Диапазон V-101: [0..80], range = 80
    // Критическая зона (2%): 80 * 0.02 = 1.6 → порог: 80 - 1.6 = 78.4
    // Зона предупреждения (5%): 80 * 0.05 = 4.0 → порог: 80 - 4.0 = 76.0

    auto res_crit = applyCommand(parameters, log, "V-101", 79.0);
    std::cout << "[Тест] Критическая зона (79.0): " << commandResultToText(res_crit) << "\n";

    auto res_warn = applyCommand(parameters, log, "V-101", 77.0);
    std::cout << "[Тест] Зона предупреждения (77.0): " << commandResultToText(res_warn) << "\n";

    auto res_err = applyCommand(parameters, log, "V-101", 100.0);
    std::cout << "[Тест] Вне диапазона (100.0): " << commandResultToText(res_err) << "\n\n";

    // --- Интерактивный режим ---
    std::string line;
    bool running = true;

    while (running) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        auto tokens = splitCommand(line);
        if (tokens.empty()) continue;

        const std::string& cmd = tokens[0];

        if (cmd == "help") {
            printHelp();

        } else if (cmd == "status") {
            printParameters(parameters);

        } else if (cmd == "set" && tokens.size() >= 3) {
            double val = std::stod(tokens[2]);
            CommandResult result = applyCommand(parameters, log, tokens[1], val);
            std::cout << commandResultToText(result) << "\n";

        } else if (cmd == "event" && tokens.size() >= 3) {
            std::string msg;
            for (size_t i = 2; i < tokens.size(); ++i) {
                if (i > 2) msg += " ";
                msg += tokens[i];
            }
            addEvent(log, tokens[1], "manual", msg);
            std::cout << "Событие добавлено.\n";

        } else if (cmd == "log") {
            printEventLog(log);

        } else if (cmd == "find" && tokens.size() >= 2) {
            auto index = findParameterIndex(parameters, tokens[1]);
            if (index) {
                const auto& p = parameters[*index];
                std::cout << "Найден: " << p.name << " = " << p.value
                          << " " << p.unit
                          << " [" << p.minValue << ".." << p.maxValue << "]\n";
            } else {
                std::cout << "Параметр \"" << tokens[1] << "\" не найден.\n";
            }

        } else if (cmd == "alarm") {
            std::size_t count = countAlarmEvents(log);
            std::cout << "Тревожных событий: " << count << "\n";
            auto alarms = getAlarmEvents(log);
            for (const auto& e : alarms) {
                std::cout << "  [" << e.id << "] " << e.severity
                          << " | " << e.source << " | " << e.message << "\n";
            }

        } else if (cmd == "sorted") {
            auto sorted = getSortedEventLog(log);
            std::cout << "Журнал (отсортированный по приоритету):\n";
            for (const auto& e : sorted) {
                std::cout << "  [" << e.id << "] " << e.severity
                          << " | " << e.source << " | " << e.message << "\n";
            }

        } else if (cmd == "stats") {
            auto stats = getEventStatistics(log);
            std::cout << "Статистика по типам событий:\n";
            std::size_t total = 0;
            for (const auto& [type, count] : stats) {
                std::cout << "  " << type << ": " << count << "\n";
                total += count;
            }
            std::cout << "  Всего: " << total << "\n";

        } else if (cmd == "summary") {
            auto summary = getParameterSummary(parameters);
            std::cout << "Краткий список параметров:\n";
            for (const auto& s : summary) {
                std::cout << "  " << s << "\n";
            }

        } else if (cmd == "visual") {
            printVisualLayers(visualLayerNames());

        } else if (cmd == "save") {
            std::filesystem::path logPath = std::filesystem::path("logs") / "dispatch_session.txt";
            saveTextLog(log, logPath);
            std::cout << "Журнал сохранён: " << logPath << "\n";
            assert(std::filesystem::exists(logPath));

        } else if (cmd == "exit") {
            running = false;

        } else {
            std::cout << "Неизвестная команда. Введите \"help\".\n";
        }
    }

    return 0;
}