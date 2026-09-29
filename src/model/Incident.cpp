#include "dispatcher/model/Incident.hpp"
#include <sstream>

Incident::Incident(std::string id, std::string description)
    : id_(std::move(id)), description_(std::move(description)), status_(ElementStatus::Warning) {}

std::string Incident::id() const { return id_; }
std::string Incident::kind() const { return "Incident"; }

std::string Incident::status() const {
    switch (status_) {
        case ElementStatus::Normal:   return "Normal";
        case ElementStatus::Warning:  return "Warning";
        case ElementStatus::Critical: return "Critical";
        case ElementStatus::Offline:  return "Offline";
    }
    return "Unknown";
}

std::string Incident::summary() const {
    std::ostringstream oss;
    oss << "Инцидент " << id_ << ": " << description_ << " (статус: " << status() << ")";
    return oss.str();
}

VisualDescriptor Incident::toVisualDescriptor() const {
    VisualDescriptor desc;
    desc.elementId = id_;
    desc.visualKind = "alarm_badge";
    desc.label = description_;

    if (status_ == ElementStatus::Critical)
        desc.color = "red";
    else
        desc.color = "orange";

    desc.priority = 5;
    return desc;
}

std::string Incident::description() const { return description_; }
void Incident::setElementStatus(ElementStatus s) { status_ = s; }