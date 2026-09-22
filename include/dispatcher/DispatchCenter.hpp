#pragma once
#include "dispatcher/DispatchUnit.hpp"
#include "dispatcher/Incident.hpp"
#include "dispatcher/EventLog.hpp"
#include <vector>

namespace dispatcher {

class DispatchCenter {
public:
    explicit DispatchCenter(EventLog& log);

    void addUnit(DispatchUnit unit);
    void addIncident(Incident incident);

    bool assign(int unitId, int incidentId);
    bool closeIncident(int incidentId);

    void printUnits() const;
    void printIncidents() const;
    void printState() const;

private:
    DispatchUnit* findUnit(int id);
    Incident* findIncident(int id);

    EventLog& log_;
    std::vector<DispatchUnit> units_;
    std::vector<Incident> incidents_;
};

} // namespace dispatcher