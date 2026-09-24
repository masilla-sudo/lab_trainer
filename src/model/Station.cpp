#include "dispatcher/model/Station.hpp"
#include <sstream>

Station::Station(std::string id, std::string name, int platforms)
    : id_(std::move(id)), name_(std::move(name)), platforms_(platforms), status_(ElementStatus::Normal) {}

std::string Station::id() const { return id_; }
std::string Station::kind() const { return "Station"; }

std::string Station::status() const {
    switch (status_) {
        case ElementStatus::Normal:   return "Normal";
        case ElementStatus::Warning:  return "Warning";
        case ElementStatus::Critical: return "Critical";
        case ElementStatus::Offline:  return "Offline";
    }
    return "Unknown";
}

std::string Station::summary() const {
    std::ostringstream oss;
    oss << "Станция " << name_ << " (" << id_ << ", платформ: " << platforms_ << ", статус: " << status() << ")";
    return oss.str();
}

VisualDescriptor Station::toVisualDescriptor() const {
    VisualDescriptor desc;
    desc.elementId = id_;
    desc.visualKind = "station_marker";
    desc.label = "Станция " + name_;

    if (status_ == ElementStatus::Critical)
        desc.color = "red";
    else if (status_ == ElementStatus::Warning)
        desc.color = "yellow";
    else
        desc.color = "green";

    desc.priority = 1;
    return desc;
}

std::string Station::name() const { return name_; }
int Station::platforms() const { return platforms_; }
void Station::setElementStatus(ElementStatus s) { status_ = s; }