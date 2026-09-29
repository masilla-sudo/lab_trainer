#pragma once
#include <ostream>

enum class SignalState {
    Red,
    Yellow,
    Green,
    Off
};

inline std::ostream& operator<<(std::ostream& os, SignalState state) {
    switch (state) {
        case SignalState::Red:    return os << "RED";
        case SignalState::Yellow: return os << "YELLOW";
        case SignalState::Green:  return os << "GREEN";
        case SignalState::Off:    return os << "OFF";
        default:                  return os << "UNKNOWN";
    }
}