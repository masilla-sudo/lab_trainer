#pragma once
#include "trainer/model.hpp"
#include <map>
#include <cstddef>
#include <filesystem>

// --- Существующие функции ---
void addEvent(EventLog& log, const std::string& severity,
              const std::string& source, const std::string& message);
void printEventLog(const EventLog& log);
void checkParametersAndLogWarnings(const ParameterList& parameters, EventLog& log);

// --- ЛР 7 (STL) ---
int severityPriority(const std::string& severity);
std::size_t countAlarmEvents(const EventLog& log);
std::vector<EventRecord> getAlarmEvents(const EventLog& log);
std::vector<EventRecord> getSortedEventLog(const EventLog& log);
std::map<std::string, std::size_t> getEventStatistics(const EventLog& log);

// --- ЛР 8: filesystem — сохранение текстового журнала ---
void saveTextLog(const EventLog& log, const std::filesystem::path& path);