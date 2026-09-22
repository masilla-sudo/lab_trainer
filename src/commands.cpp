#include "trainer/commands.hpp"
#include <iostream>
#include <sstream>

std::vector<std::string> splitCommand(const std::string& line) {
    std::istringstream input(line);
    std::vector<std::string> tokens;
    std::string token;
    while (input >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void printHelp() {
    std::cout << "Доступные команды:\n"
              << "  help          - справка по командам\n"
              << "  status        - текущие параметры узла\n"
              << "  set <param> <value> - изменить параметр\n"
              << "  event <sev> <msg> - добавить событие вручную\n"
              << "  log           - показать журнал событий\n"
              << "  visual        - список визуальных/AR-слоёв\n"
              << "  exit          - завершить работу\n";
}

void printVisualLayers(const std::array<std::string, 5>& layers) {
    std::cout << "Визуальные/AR-слои (транспортный узел):\n";
    for (size_t i = 0; i < layers.size(); ++i) {
        std::cout << (i + 1) << ". " << layers[i] << "\n";
    }
}