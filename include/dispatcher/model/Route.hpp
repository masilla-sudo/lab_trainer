#pragma once
#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include "dispatcher/model/ElementStatus.hpp"
#include <string>

class Route : public ISystemElement, public IVisualDescriptorProvider {
public:
    Route(std::string id, std::string from, std::string to);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    std::string from() const;
    std::string to() const;
    void setElementStatus(ElementStatus s);

private:
    std::string id_;
    std::string from_;
    std::string to_;
    ElementStatus status_;
};