#include "dispatcher/DispatchCenter.hpp"
#include <iostream>

namespace dispatcher {

DispatchCenter::DispatchCenter(EventLog& log) : log_(log) {}

void DispatchCenter::addUnit(DispatchUnit unit) {
    units_.push_back(std::move(unit));
    log_.write("unit added: " + std::to_string(unit.id()));
}

void DispatchCenter::addIncident(Incident incident) {
    incidents_.push_back(std::move(incident));
    log_.write("incident added: " + std::to_string(incident.id()));
}

DispatchUnit* DispatchCenter::findUnit(int id) {
    for (auto& u : units_) {
        if (u.id() == id) return &u;
    }
    return nullptr;
}

Incident* DispatchCenter::findIncident(int id) {
    for (auto& i : incidents_) {
        if (i.id() == id) return &i;
    }
    return nullptr;
}

bool DispatchCenter::assign(int unitId, int incidentId) {
    auto* unit = findUnit(unitId);
    auto* incident = findIncident(incidentId);

    if (!unit || !incident) {
        log_.write("assign failed: unit or incident not found");
        return false;
    }

    if (unit->status() != UnitStatus::Available) {
        log_.write("assign failed: unit not available");
        return false;
    }

    if (incident->status() != IncidentStatus::Open) {
        log_.write("assign failed: incident not open");
        return false;
    }

    if (unit->zone() != incident->zone()) {
        log_.write("assign warning: unit and incident zones differ");
    }

    if (unit->assignToIncident(incidentId) && incident->assignUnit(unitId)) {
        log_.write("assignment successful: unit " + std::to_string(unitId) +
                   " to incident " + std::to_string(incidentId));
        return true;
    }

    log_.write("assign failed: internal error");
    return false;
}

bool DispatchCenter::closeIncident(int incidentId) {
    auto* incident = findIncident(incidentId);
    if (!incident) {
        log_.write("close failed: incident not found");
        return false;
    }

    if (incident->status() == IncidentStatus::Closed) {
        log_.write("close failed: incident already closed");
        return false;
    }

    incident->close();
    log_.write("incident closed: " + std::to_string(incidentId));
    return true;
}

void DispatchCenter::printUnits() const {
    std::cout << "Units:\n";
    for (const auto& u : units_) {
        std::cout << "  ID: " << u.id()
                  << " | Name: " << u.name()
                  << " | Zone: " << u.zone()
                  << " | Status: " << toString(u.status())
                  << " | Assigned: "
                  << (u.assignedIncidentId() ? std::to_string(*u.assignedIncidentId()) : "none")
                  << "\n";
    }
}

void DispatchCenter::printIncidents() const {
    std::cout << "Incidents:\n";
    for (const auto& i : incidents_) {
        std::cout << "  ID: " << i.id()
                  << " | Title: " << i.title()
                  << " | Zone: " << i.zone()
                  << " | Severity: " << toString(i.severity())
                  << " | Status: " << toString(i.status())
                  << " | Marker: " << toString(i.marker())
                  << " | Assigned: "
                  << (i.assignedUnitId() ? std::to_string(*i.assignedUnitId()) : "none")
                  << "\n";
    }
}

void DispatchCenter::printState() const {
    printUnits();
    printIncidents();
    std::cout << "---\n";
}

} // namespace dispatcher