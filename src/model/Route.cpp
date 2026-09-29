#include "dispatcher/model/Route.hpp"
#include <sstream>

Route::Route(std::string id, std::string from, std::string to)
    : id_(std::move(id)), from_(std::move(from)), to_(std::move(to)), status_(ElementStatus::Normal) {}

std::string Route::id() const { return id_; }
std::string Route::kind() const { return "Route"; }

std::string Route::status() const {
    switch (status_) {
        case ElementStatus::Normal:   return "Normal";
        case ElementStatus::Warning:  return "Warning";
        case ElementStatus::Critical: return "Critical";
        case ElementStatus::Offline:  return "Offline";
    }
    return "Unknown";
}

std::string Route::summary() const {
    std::ostringstream oss;
    oss << "Маршрут " << id_ << ": " << from_ << " -> " << to_ << " (статус: " << status() << ")";
    return oss.str();
}

VisualDescriptor Route::toVisualDescriptor() const {
    VisualDescriptor desc;
    desc.elementId = id_;
    desc.visualKind = "route_line";
    desc.label = from_ + " -> " + to_;

    if (status_ == ElementStatus::Critical)
        desc.color = "red";
    else if (status_ == ElementStatus::Warning)
        desc.color = "orange";
    else
        desc.color = "blue";

    desc.priority = 3;
    return desc;
}

std::string Route::from() const { return from_; }
std::string Route::to() const { return to_; }
void Route::setElementStatus(ElementStatus s) { status_ = s; }