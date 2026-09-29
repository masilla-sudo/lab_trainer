#pragma once
#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include "dispatcher/model/ElementStatus.hpp"
#include <string>

class ARHint : public ISystemElement, public IVisualDescriptorProvider {
public:
    ARHint(std::string id, std::string hintText, std::string anchorId);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    std::string hintText() const;
    std::string anchorId() const;

private:
    std::string id_;
    std::string hintText_;
    std::string anchorId_;
    ElementStatus status_;
};