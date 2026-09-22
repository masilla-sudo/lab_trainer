#include "dispatcher/Incident.hpp"
#include <stdexcept>

namespace dispatcher {

Incident::Incident(int id, std::string title, std::string zone,
                   Severity severity, VisualMarker marker)
    : id_{id}, title_{std::move(title)}, zone_{std::move(zone)},
      severity_{severity}, marker_{marker} {
    if (id <= 0) {
        throw std::invalid_argument("Incident ID must be positive");
    }
    if (title_.empty()) {
        throw std::invalid_argument("Incident title cannot be empty");
    }
    if (zone_.empty()) {
        throw std::invalid_argument("Incident zone cannot be empty");
    }
}

int Incident::id() const { return id_; }
const std::string& Incident::title() const { return title_; }
const std::string& Incident::zone() const { return zone_; }
Severity Incident::severity() const { return severity_; }
IncidentStatus Incident::status() const { return status_; }
VisualMarker Incident::marker() const { return marker_; }
std::optional<int> Incident::assignedUnitId() const {
    return assignedUnitId_;
}

bool Incident::assignUnit(int unitId) {
    if (status_ != IncidentStatus::Open) {
        return false;
    }
    status_ = IncidentStatus::Assigned;
    assignedUnitId_ = unitId;
    return true;
}

void Incident::close() {
    if (status_ == IncidentStatus::Closed) {
        return;
    }
    status_ = IncidentStatus::Closed;
    assignedUnitId_.reset();
}

} // namespace dispatcher