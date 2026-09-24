#pragma once
#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include "dispatcher/model/ElementStatus.hpp"
#include <string>

class Incident : public ISystemElement, public IVisualDescriptorProvider {
public:
    Incident(std::string id, std::string description);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    std::string description() const;
    void setElementStatus(ElementStatus s);

private:
    std::string id_;
    std::string description_;
    ElementStatus status_;
};