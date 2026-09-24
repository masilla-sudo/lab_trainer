#pragma once

#include <string>

// Состояние дискретного сигнала — пример параметра с перечислимым типом.
// Используется для клапанов, насосов, выключателей и т.п.
enum class SignalState {
    Off,
    On,
    Fault
};

// Строковое представление состояния для вывода и DTO.
inline std::string toString(SignalState s)
{
    switch (s) {
        case SignalState::Off:   return "OFF";
        case SignalState::On:    return "ON";
        case SignalState::Fault: return "FAULT";
    }
    return "UNKNOWN";
}