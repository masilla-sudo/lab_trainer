#include "dispatcher/EventLog.hpp"
#include <iostream>
#include <stdexcept>

namespace dispatcher {

EventLog::EventLog(const std::filesystem::path& path)
    : out_(path, std::ios::app)
{
    if (!out_) {
        throw std::runtime_error("Cannot open event log file: " + path.string());
    }
    write("log opened");
}

EventLog::~EventLog() {
    if (out_) {
        out_ << "log closed\n";
    }
}

void EventLog::write(std::string_view message) {
    if (out_) {
        out_ << message << '\n';
    }
}

} // namespace dispatcher