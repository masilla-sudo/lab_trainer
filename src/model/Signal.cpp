#include "dispatcher/model/Signal.hpp"
#include <sstream>

Signal::Signal(std::string id, SignalState state)
    : id_(std::move(id)), state_(state) {}

std::string Signal::id() const { return id_; }
std::string Signal::kind() const { return "Signal"; }

std::string Signal::status() const {
    switch (state_) {
        case SignalState::Green:  return "Normal";
        case SignalState::Yellow: return "Warning";
        case SignalState::Red:    return "Critical";
    }
    return "Unknown";
}

std::string Signal::summary() const {
    std::ostringstream oss;
    oss << "Сигнал " << id_ << " (состояние: " << status() << ")";
    return oss.str();
}

VisualDescriptor Signal::toVisualDescriptor() const {
    VisualDescriptor desc;
    desc.elementId = id_;
    desc.visualKind = "marker";
    desc.label = "Сигнал " + id_;

    switch (state_) {
        case SignalState::Green: desc.color = "green"; break;
        case SignalState::Yellow: desc.color = "yellow"; break;
        case SignalState::Red: desc.color = "red"; break;
    }
    desc.priority = 1;
    return desc;
}

SignalState Signal::state() const { return state_; }
void Signal::setState(SignalState newState) { state_ = newState; }