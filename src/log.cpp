#include "trainer/log.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>

// --- Вспомогательная функция: приоритет по строковому типу ---
int severityPriority(const std::string& severity) {
    if (severity == "Alarm")          return 3;
    if (severity == "Warning")       return 2;
    if (severity == "OperatorAction") return 1;
    return 0;  // Info и прочие
}

// --- Добавить событие в журнал (id = текущий размер) ---
void addEvent(EventLog& log, const std::string& severity,
              const std::string& source, const std::string& message) {
    log.push_back({log.size(), severity, source, message});
}

// --- Вывод журнала ---
void printEventLog(const EventLog& log) {
    std::cout << "Журнал событий (" << log.size() << "):\n";
    for (const auto& e : log) {
        std::cout << "  [" << e.id << "] " << e.severity
                  << " | " << e.source << " | " << e.message << "\n";
    }
}

// --- Проверка параметров и запись предупреждений ---
void checkParametersAndLogWarnings(const ParameterList& parameters, EventLog& log) {
    for (const auto& p : parameters) {
        if (!isInRange(p)) {
            addEvent(log, "Warning", p.name,
                     "Значение вне диапазона [" + std::to_string(p.minValue) +
                     ".." + std::to_string(p.maxValue) + "]");
        }
    }
}

// --- count_if: подсчёт тревожных событий ---
std::size_t countAlarmEvents(const EventLog& log) {
    return std::count_if(log.begin(), log.end(),
        [](const EventRecord& e) {    // lambda #4
            return e.severity == "Warning" || e.severity == "Alarm";
        });
}

// --- copy_if: список тревожных событий ---
std::vector<EventRecord> getAlarmEvents(const EventLog& log) {
    std::vector<EventRecord> result;
    std::copy_if(log.begin(), log.end(), std::back_inserter(result),
        [](const EventRecord& e) {    // lambda #5
            return e.severity == "Warning" || e.severity == "Alarm";
        });
    return result;
}

// --- sort: отсортированная копия журнала ---
std::vector<EventRecord> getSortedEventLog(const EventLog& log) {
    std::vector<EventRecord> sorted = log;   // копия
    std::sort(sorted.begin(), sorted.end(),
        [](const EventRecord& left, const EventRecord& right) {    // lambda #6
            int pl = severityPriority(left.severity);
            int pr = severityPriority(right.severity);
            if (pl != pr) return pl > pr;       // по приоритету (убывание)
            return left.id < right.id;           // затем по id (возрастание)
        });
    return sorted;
}

// --- map: статистика по типам событий ---
std::map<std::string, std::size_t> getEventStatistics(const EventLog& log) {
    std::map<std::string, std::size_t> stats;
    for (const auto& e : log) {
        ++stats[e.severity];
    }
    return stats;
}
#include <filesystem>
#include <fstream>

// ... существующие функции ...

// --- ЛР 8: std::filesystem — сохранение текстового журнала ---
void saveTextLog(const EventLog& log, const std::filesystem::path& path) {
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }
    std::ofstream out(path);
    for (const auto& e : log) {
        out << "[" << e.id << "] " << e.severity
            << " | " << e.source << " | " << e.message << "\n";
    }
}