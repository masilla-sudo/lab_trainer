#include "dispatcher/visual/ConsoleVisualRenderer.hpp"
#include <iostream>
#include <iomanip>

void ConsoleVisualRenderer::render(const std::vector<VisualDescriptor>& descriptors) const {
    std::cout << "\n--- Визуальные описатели (для клиента/AR) ---\n";
    std::cout << std::left
              << std::setw(12) << "ID"
              << std::setw(16) << "Тип"
              << std::setw(24) << "Подпись"
              << std::setw(8) << "Цвет"
              << "Приоритет"
              << (descriptors.empty() ? "" : "\n");

    for (const auto& d : descriptors) {
        std::cout << std::left
                  << std::setw(12) << d.elementId
                  << std::setw(16) << d.visualKind
                  << std::setw(24) << d.label
                  << std::setw(8) << d.color
                  << d.priority
                  << "\n";
    }

    if (descriptors.empty()) {
        std::cout << "(нет визуальных описателей)\n";
    }
}