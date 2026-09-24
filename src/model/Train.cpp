#include "dispatcher/model/Train.hpp"
#include <sstream>

Train::Train(std::string id, std::string route)
    : id_(std::move(id)), route_(std::move(route)), status_(ElementStatus::Normal) {}

std::string Train::id() const { return id_; }
std::string Train::kind() const { return "Train"; }

std::string Train::status() const {
    switch (status_) {
        case ElementStatus::Normal:   return "Normal";
        case ElementStatus::Warning:  return "Warning";
        case ElementStatus::Critical: return "Critical";
        case ElementStatus::Offline:  return "Offline";
    }
    return "Unknown";
}

std::string Train::summary() const {
    std::ostringstream oss;
    oss << "Поезд " << id_ << " на маршруте \"" << route_ << "\" (статус: " << status() << ")";
    return oss.str();
}

VisualDescriptor Train::toVisualDescriptor() const {
    VisualDescriptor desc;
    desc.elementId = id_;
    desc.visualKind = "train_marker";
    desc.label = "Поезд " + id_ + " (" + route_ + ")";

    if (status_ == ElementStatus::Critical)
        desc.color = "red";
    else if (status_ == ElementStatus::Warning)
        desc.color = "yellow";
    else
        desc.color = "blue";

    desc.priority = 2;
    return desc;
}

std::string Train::route() const { return route_; }
void Train::setRoute(std::string newRoute) { route_ = std::move(newRoute); }
ElementStatus Train::elementStatus() const { return status_; }
void Train::setElementStatus(ElementStatus s) { status_ = s; }
