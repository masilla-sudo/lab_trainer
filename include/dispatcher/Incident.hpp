#pragma once
#include "dispatcher/DispatchTypes.hpp"
#include <optional>
#include <string>

namespace dispatcher {

class Incident {
public:
    Incident(int id, std::string title, std::string zone,
             Severity severity, VisualMarker marker);

    int id() const;
    const std::string& title() const;
    const std::string& zone() const;
    Severity severity() const;
    IncidentStatus status() const;
    VisualMarker marker() const;
    std::optional<int> assignedUnitId() const;

    bool assignUnit(int unitId);
    void close();

private:
    int id_{};
    std::string title_;
    std::string zone_;
    Severity severity_{Severity::Low};
    IncidentStatus status_{IncidentStatus::Open};
    VisualMarker marker_{VisualMarker::None};
    std::optional<int> assignedUnitId_;
};

} // namespace dispatcher