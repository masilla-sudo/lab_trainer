#include "dispatcher/Utils.hpp"

namespace dispatcher {

std::string_view toString(UnitStatus status) {
    switch (status) {
        case UnitStatus::Available:   return "Available";
        case UnitStatus::Busy:       return "Busy";
        case UnitStatus::Maintenance: return "Maintenance";
        case UnitStatus::Offline:    return "Offline";
    }
    return "Unknown";
}

std::string_view toString(IncidentStatus status) {
    switch (status) {
        case IncidentStatus::Open:     return "Open";
        case IncidentStatus::Assigned: return "Assigned";
        case IncidentStatus::Closed:   return "Closed";
    }
    return "Unknown";
}

std::string_view toString(Severity severity) {
    switch (severity) {
        case Severity::Low:      return "Low";
        case Severity::Medium:   return "Medium";
        case Severity::High:     return "High";
        case Severity::Critical: return "Critical";
    }
    return "Unknown";
}

std::string_view toString(VisualMarker marker) {
    switch (marker) {
        case VisualMarker::None:        return "None";
        case VisualMarker::Alert:       return "Alert";
        case VisualMarker::Route:      return "Route";
        case VisualMarker::Repair:      return "Repair";
        case VisualMarker::Evacuation:  return "Evacuation";
        case VisualMarker::Sensor:      return "Sensor";
    }
    return "Unknown";
}

} // namespace dispatcher
