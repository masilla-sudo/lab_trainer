#include <iostream>                          // Подключает потоки ввода-вывода для вывода в консоль и вывода ошибок
#include <filesystem>                        // Подключает библиотеку для работы с путями (std::filesystem::path)
#include "dispatcher/DispatchTypes.hpp"      // Подключает перечисления: UnitStatus, IncidentStatus, Severity, VisualMarker
#include "dispatcher/DispatchUnit.hpp"       // Подключает класс DispatchUnit — подразделение (бригада, дрон, техник)
#include "dispatcher/Incident.hpp"           // Подключает класс Incident — инцидент (происшествие, авария)
#include "dispatcher/EventLog.hpp"          // Подключает класс EventLog — запись журнала событий в файл
#include "dispatcher/DispatchCenter.hpp"    // Подключает класс DispatchCenter — центр диспетчеризации (главный управляющий класс)

int main() {                                  // Точка входа в программу — отсюда начинается выполнение
    namespace fs = std::filesystem;         // Сокращение: вместо std::filesystem::path можно писать fs::path

    try {                                    // Открывает блок try — все исключения будут перехвачены ниже
        dispatcher::EventLog log{fs::path("dispatch.log")};  // Создаёт журнал событий: файл dispatch.log в текущей папке (режим append)
        dispatcher::DispatchCenter center{log};             // Создаёт центр диспетчеризации, передаём ему ссылку на журнал

        // --- Добавляем подразделения в систему ---
        center.addUnit(dispatcher::DispatchUnit{1, "Brigade A", "north"});    // Бригада A, зона «север», ID = 1
        center.addUnit(dispatcher::DispatchUnit{2, "Drone 1", "center"});    // Дрон 1, зона «центр», ID = 2
        center.addUnit(dispatcher::DispatchUnit{3, "Technician B", "north"}); // Техник B, зона «север», ID = 3

        // --- Добавляем инциденты ---
        center.addIncident(dispatcher::Incident{       // Инцидент: задымление у станции
            100,                                        // ID инцидента — 100
            "Smoke detected near station",               // Описание: обнаружен дым
            "north",                                    // Зона: север (совпадает с Brigade A и Technician B)
            dispatcher::Severity::High,                 // Уровень серьёзности: высокий
            dispatcher::VisualMarker::Alert             // Визуальный маркер: тревога
        });

        center.addIncident(dispatcher::Incident{       // Инцидент: отключение питания в секторе C
            101,                                        // ID инцидента — 101
            "Power outage in sector C",                 // Описание: нет электричества
            "center",                                   // Зона: центр (совпадает с Drone 1)
            dispatcher::Severity::Critical,             // Уровень серьёзности: критический
            dispatcher::VisualMarker::Repair            // Визуальный маркер: требуется ремонт
        });

        std::cout << "Initial state:\n";               // Выводит заголовок: начальное состояние системы
        center.printState();                           // Печатает все подразделения, все инциденты и разделитель

        std::cout << "Assigning unit 1 to incident 100:\n";  // Заголовок: назначаем бригаду A на задымление
        center.assign(1, 100);                         // Назначает подразделение 1 на инцидент 100 (зоны совпадают — успешно)
        center.printState();                           // Печатает обновлённое состояние (unit 1 → Busy, incident 100 → Assigned)

        std::cout << "Assigning unit 2 to incident 101:\n";  // Заголовок: назначаем дрон на аварию питания
        center.assign(2, 101);                         // Назначает подразделение 2 на инцидент 101 (зоны совпадают — успешно)
        center.printState();                           // Печатает обновлённое состояние (unit 2 → Busy, incident 101 → Assigned)

        std::cout << "Closing incident 100:\n";        // Заголовок: закрываем инцидент с задымлением
        center.closeIncident(100);                    // Закрывает инцидент 100 (status → Closed, assignedUnitId сброшен)
        center.printState();                           // Печатает состояние (incident 100 → Closed, но unit 1 всё ещё Busy!)

        std::cout << "Trying to assign unit 1 (now busy) to incident 101:\n";  // Заголовок: пытаемся снова назначить бригаду A
        center.assign(1, 101);                         // Попытка назначить unit 1 на incident 101 — должна провалиться (unit занят)
        center.printState();                           // Печатает состояние — ничего не изменилось (назначение отклонено)

    } catch (const std::exception& e) {               // Перехватывает любые исключения, выброшенные в блоке try
        std::cerr << "Error: " << e.what() << "\n";   // Выводит сообщение об ошибке в поток stderr (красным в консоли)
        return 1;                                     // Возвращает код ошибки — 1 (ненулевой = аварийное завершение)
    }

    return 0;                                          // Возвращает 0 — программа завершилась успешно
}