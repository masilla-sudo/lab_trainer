#pragma once
#include <vector>
#include "dispatcher/visual/VisualDescriptor.hpp"

class ConsoleVisualRenderer {
public:
    void render(const std::vector<VisualDescriptor>& descriptors) const;
};
