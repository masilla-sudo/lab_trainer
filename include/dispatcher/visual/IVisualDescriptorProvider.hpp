#pragma once
#include "dispatcher/visual/VisualDescriptor.hpp"

class IVisualDescriptorProvider {
public:
    virtual ~IVisualDescriptorProvider() = default;
    virtual VisualDescriptor toVisualDescriptor() const = 0;
};