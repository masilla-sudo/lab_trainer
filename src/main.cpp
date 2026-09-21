#include "trainer_logic.hpp"
#include <iostream>

int read_menu_item() {
    std::cout << "\n1. Показать состояние\n";
    std::cout << "2. Изменить загруженность маршрута\n";
    std::cout << "3. Отправить команду диспетчера\n";
    std::cout << "4. Пересчитать состояние\n";
    std::cout << "0. Выход\n";
    std::cout << "Выбор: ";
    int item = -1;
    std::cin >> item;
    return item;
}

DispatcherCommand read_command() {
    std::cout << "\nКоманды диспетчера:\n";
    std::cout << "1. Подтвердить нарушение\n";
    std::cout << "2. Снизить загруженность\n";
    std::cout << "3. Восстановить связь\n";
    std::cout << "4. Сбросить сценарий\n";
    std::cout << "Выбор: ";
    int cmd = -1;
    std::cin >> cmd;
    switch (cmd) {
        case 1:  return DispatcherCommand::AcknowledgeViolation;
        case 2:  return DispatcherCommand::ReduceCongestion;
        case 3:  return DispatcherCommand::RestoreConnection;
        case 4:  return DispatcherCommand::ResetScenario;
        default: return DispatcherCommand::None;
    }
}

int main() {
    double congestion_percent = 62.0;
    bool connection_ok = true;
    int active_violations = 1;
    SituationState state = calculate_state(congestion_percent, connection_ok, active_violations);

    bool running = true;
    while (running) {
        int item = read_menu_item();
        switch (item) {
            case 1:
                print_status(congestion_percent, connection_ok, active_violations, state);
                break;
            case 2: {
                std::cout << "Новая загруженность, %: ";
                double old = congestion_percent;
                std::cin >> congestion_percent;
                if (!is_congestion_valid(congestion_percent)) {
                    std::cout << "Ошибка: значение должно быть от 0 до 100.\n";
                    congestion_percent = old;
                }
                break;
            }
            case 3: {
                DispatcherCommand command = read_command();
                if (command != DispatcherCommand::None) {
                    apply_command(command, congestion_percent, connection_ok, active_violations);
                    std::cout << "Команда применена.\n";
                } else {
                    std::cout << "Неизвестная команда.\n";
                }
                break;
            }
            case 4:
                state = calculate_state(congestion_percent, connection_ok, active_violations);
                std::cout << "Состояние пересчитано.\n";
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Неизвестный пункт меню.\n";
        }
    }
}