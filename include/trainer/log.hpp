#pragma once
#include "trainer/model.hpp"

void addEvent(EventLog& log, const std::string& severity,
              const std::string& source, const std::string& message);
void printEventLog(const EventLog& log);
void checkParametersAndLogWarnings(const ParameterList& parameters, EventLog& log);