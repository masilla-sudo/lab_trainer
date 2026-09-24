#pragma once
#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include "dispatcher/model/ElementStatus.hpp"
#include <string>

enum class SignalState {
    Green,
    Yellow,
    Red
};

class Signal : public ISystemElement, public IVisualDescriptorProvider {
public:
    Signal(std::string id, SignalState state);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    SignalState state() const;
    void setState(SignalState newState);

private:
    std::string id_;
    SignalState state_;
};
