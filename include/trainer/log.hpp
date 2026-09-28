#pragma once
#include "trainer/model.hpp"
#include <map>
#include <cstddef>

// --- Существующие функции ---
void addEvent(EventLog& log, const std::string& severity,
              const std::string& source, const std::string& message);
void printEventLog(const EventLog& log);
void checkParametersAndLogWarnings(const ParameterList& parameters, EventLog& log);

// --- Новые функции ЛР 7 (STL-алгоритмы) ---
// Приоритет по строковому типу (Alarm=3, Warning=2, OperatorAction=1, Info=0)
int severityPriority(const std::string& severity);
// count_if: подсчёт тревожных событий
std::size_t countAlarmEvents(const EventLog& log);
// copy_if: список тревожных событий
std::vector<EventRecord> getAlarmEvents(const EventLog& log);
// sort: отсортированная копия журнала (по приоритету, затем по id)
std::vector<EventRecord> getSortedEventLog(const EventLog& log);
// map: статистика по типам событий
std::map<std::string, std::size_t> getEventStatistics(const EventLog& log);