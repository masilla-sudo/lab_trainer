#include "dispatcher/DispatchUnit.hpp"
#include <stdexcept>

namespace dispatcher {

DispatchUnit::DispatchUnit(int id, std::string name, std::string zone)
    : id_{id}, name_{std::move(name)}, zone_{std::move(zone)} {
    if (id <= 0) {
        throw std::invalid_argument("Unit ID must be positive");
    }
    if (name_.empty()) {
        throw std::invalid_argument("Unit name cannot be empty");
    }
    if (zone_.empty()) {
        throw std::invalid_argument("Unit zone cannot be empty");
    }
}

int DispatchUnit::id() const { return id_; }
const std::string& DispatchUnit::name() const { return name_; }
const std::string& DispatchUnit::zone() const { return zone_; }
UnitStatus DispatchUnit::status() const { return status_; }
std::optional<int> DispatchUnit::assignedIncidentId() const {
    return assignedIncidentId_;
}

bool DispatchUnit::assignToIncident(int incidentId) {
    if (status_ != UnitStatus::Available) {
        return false;
    }
    status_ = UnitStatus::Busy;
    assignedIncidentId_ = incidentId;
    return true;
}

void DispatchUnit::release() {
    status_ = UnitStatus::Available;
    assignedIncidentId_.reset();
}

void DispatchUnit::setMaintenance() {
    status_ = UnitStatus::Maintenance;
    assignedIncidentId_.reset();
}

void DispatchUnit::setOffline() {
    status_ = UnitStatus::Offline;
    assignedIncidentId_.reset();
}

} // namespace dispatcher