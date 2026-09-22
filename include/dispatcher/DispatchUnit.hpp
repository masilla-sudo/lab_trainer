#pragma once
#include "dispatcher/DispatchTypes.hpp"
#include <optional>
#include <string>

namespace dispatcher {

class DispatchUnit {
public:
    DispatchUnit(int id, std::string name, std::string zone);

    int id() const;
    const std::string& name() const;
    const std::string& zone() const;
    UnitStatus status() const;
    std::optional<int> assignedIncidentId() const;

    bool assignToIncident(int incidentId);
    void release();
    void setMaintenance();
    void setOffline();

private:
    int id_{};
    std::string name_;
    std::string zone_;
    UnitStatus status_{UnitStatus::Available};
    std::optional<int> assignedIncidentId_;
};

} // namespace dispatcher
