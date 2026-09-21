#include "trainer_logic.hpp"
#include <iostream>

bool is_congestion_valid(double congestion_percent) {
    return congestion_percent >= 0.0 && congestion_percent <= 100.0;
}

SituationState calculate_state(double congestion_percent,
                               bool connection_ok,
                               int active_violations) {
    if (!connection_ok || active_violations >= 3 || congestion_percent >= 90.0) {
        return SituationState::Emergency;
    }
    if (active_violations > 0 || congestion_percent >= 75.0) {
        return SituationState::Warning;
    }
    return SituationState::Normal;
}

void print_state(SituationState state) {
    switch (state) {
        case SituationState::Normal:
            std::cout << "Норма"; break;
        case SituationState::Warning:
            std::cout << "Предупреждение"; break;
        case SituationState::Emergency:
            std::cout << "Аварийная ситуация"; break;
    }
}

void print_status(double congestion_percent,
                  bool connection_ok,
                  int active_violations,
                  SituationState state) {
    std::cout << "Загруженность маршрута: " << congestion_percent << "%\n";
    std::cout << "Связь с объектом: " << (connection_ok ? "OK" : "НЕТ") << "\n";
    std::cout << "Активных нарушений: " << active_violations << "\n";
    std::cout << "Состояние: ";
    print_state(state);
    std::cout << "\n";
}

void apply_command(DispatcherCommand command,
                   double& congestion_percent,
                   bool& connection_ok,
                   int& active_violations) {
    switch (command) {
        case DispatcherCommand::AcknowledgeViolation:
            if (active_violations > 0) {
                --active_violations;
            }
            break;
        case DispatcherCommand::ReduceCongestion:
            congestion_percent -= 10.0;
            if (congestion_percent < 0.0) {
                congestion_percent = 0.0;
            }
            break;
        case DispatcherCommand::RestoreConnection:
            connection_ok = true;
            break;
        case DispatcherCommand::ResetScenario:
            congestion_percent = 50.0;
            connection_ok = true;
            active_violations = 0;
            break;
        case DispatcherCommand::None:
            break;
    }
}