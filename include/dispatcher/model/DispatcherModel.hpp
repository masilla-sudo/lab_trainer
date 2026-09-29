#pragma once
#include <memory>
#include <vector>
#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/VisualDescriptor.hpp"

class DispatcherModel {
public:
    void addElement(std::unique_ptr<ISystemElement> element);
    void printSummary() const;
    std::vector<VisualDescriptor> collectVisualDescriptors() const;

private:
    std::vector<std::unique_ptr<ISystemElement>> elements_;
};