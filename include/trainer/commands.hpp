#pragma once
#include <string>
#include <vector>
#include <array>

std::vector<std::string> splitCommand(const std::string& line);
void printHelp();
void printVisualLayers(const std::array<std::string, 5>& layers);