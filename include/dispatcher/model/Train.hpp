#pragma once
#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include "dispatcher/model/ElementStatus.hpp"
#include <string>

class Train : public ISystemElement, public IVisualDescriptorProvider {
public:
    Train(std::string id, std::string route);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    std::string route() const;
    void setRoute(std::string newRoute);
    ElementStatus elementStatus() const;
    void setElementStatus(ElementStatus s);

private:
    std::string id_;
    std::string route_;
    ElementStatus status_;
};