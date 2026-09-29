#include "dispatcher/model/ARHint.hpp"
#include <sstream>

ARHint::ARHint(std::string id, std::string hintText, std::string anchorId)
    : id_(std::move(id)),
      hintText_(std::move(hintText)),
      anchorId_(std::move(anchorId)),
      status_(ElementStatus::Warning) {}

std::string ARHint::id() const { return id_; }
std::string ARHint::kind() const { return "ARHint"; }

std::string ARHint::status() const {
    switch (status_) {
        case ElementStatus::Normal:   return "Normal";
        case ElementStatus::Warning:  return "Warning";
        case ElementStatus::Critical: return "Critical";
        case ElementStatus::Offline:  return "Offline";
    }
    return "Unknown";
}

std::string ARHint::summary() const {
    std::ostringstream oss;
    oss << "AR-подсказка " << id_ << ": \"" << hintText_
        << "\" (для элемента " << anchorId_ << ", статус: " << status() << ")";
    return oss.str();
}

VisualDescriptor ARHint::toVisualDescriptor() const {
    VisualDescriptor desc;
    desc.elementId = id_;
    desc.visualKind = "ar_hint";
    desc.label = hintText_;
    desc.anchorId = anchorId_;

    if (status_ == ElementStatus::Critical)
        desc.color = "red";
    else
        desc.color = "cyan";

    desc.priority = 10;
    return desc;
}

std::string ARHint::hintText() const { return hintText_; }
std::string ARHint::anchorId() const { return anchorId_; }
