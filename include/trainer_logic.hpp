#pragma once

enum class SituationState {
    Normal,
    Warning,
    Emergency
};

enum class DispatcherCommand {
    None,
    AcknowledgeViolation,
    ReduceCongestion,
    RestoreConnection,
    ResetScenario
};

bool is_congestion_valid(double congestion_percent);
SituationState calculate_state(double congestion_percent,
                               bool connection_ok,
                               int active_violations);
void print_state(SituationState state);
void print_status(double congestion_percent,
                  bool connection_ok,
                  int active_violations,
                  SituationState state);
void apply_command(DispatcherCommand command,
                   double& congestion_percent,
                   bool& connection_ok,
                   int& active_violations);