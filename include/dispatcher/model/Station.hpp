#pragma once
#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include "dispatcher/model/ElementStatus.hpp"
#include <string>

class Station : public ISystemElement, public IVisualDescriptorProvider {
public:
    Station(std::string id, std::string name, int platforms);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    std::string name() const;
    int platforms() const;
    void setElementStatus(ElementStatus s);

private:
    std::string id_;
    std::string name_;
    int platforms_;
    ElementStatus status_;
};