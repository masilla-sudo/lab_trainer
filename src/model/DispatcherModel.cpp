#include "dispatcher/model/DispatcherModel.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include <iostream>

void DispatcherModel::addElement(std::unique_ptr<ISystemElement> element) {
    elements_.push_back(std::move(element));
}

void DispatcherModel::printSummary() const {
    std::cout << "--- Сводка элементов системы ---\n";
    for (const auto& el : elements_) {
        std::cout << el->summary() << "\n";
    }
}

std::vector<VisualDescriptor> DispatcherModel::collectVisualDescriptors() const {
    std::vector<VisualDescriptor> result;
    result.reserve(elements_.size());
    for (const auto& el : elements_) {
        if (auto* provider = dynamic_cast<const IVisualDescriptorProvider*>(el.get())) {
            result.push_back(provider->toVisualDescriptor());
        }
    }
    return result;
}