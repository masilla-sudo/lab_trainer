#include "dispatcher/DispatchCenter.hpp"  // Подключает объявление класса DispatchCenter и всех используемых типов
#include <iostream>                       // Подключает потоки ввода-вывода (std::cout) для вывода состояния в консоль

namespace dispatcher {                    // Пространство имён для компонентов диспетчерской системы

// Конструктор: инициализирует центр диспетчеризации, сохраняя ссылку на журнал событий
DispatchCenter::DispatchCenter(EventLog& log) : log_(log) {}

// Добавляет новое подразделение в систему и фиксирует событие в логе
void DispatchCenter::addUnit(DispatchUnit unit) {
    units_.push_back(std::move(unit));    // Перемещает объект подразделения в вектор (без лишнего копирования)
    log_.write("unit added: " + std::to_string(unit.id()));  // Записывает в лог факт добавления подразделения по его ID
}

// Добавляет новый инцидент в систему и фиксирует событие в логе
void DispatchCenter::addIncident(Incident incident) {
    incidents_.push_back(std::move(incident));  // Перемещает объект инцидента в вектор (экономия памяти и времени)
    log_.write("incident added: " + std::to_string(incident.id()));  // Записывает в лог факт добавления инцидента по его ID
}

// Ищет подразделение по ID и возвращает указатель на него (или nullptr, если не найдено)
DispatchUnit* DispatchCenter::findUnit(int id) {
    for (auto& u : units_) {                // Проходит по всем подразделениям в списке
        if (u.id() == id) return &u;       // Если ID совпал — возвращает адрес объекта
    }
    return nullptr;                        // Если ничего не найдено — возвращает пустой указатель
}

// Ищет инцидент по ID и возвращает указатель на него (или nullptr, если не найден)
Incident* DispatchCenter::findIncident(int id) {
    for (auto& i : incidents_) {            // Проходит по всем инцидентам в списке
        if (i.id() == id) return &i;        // Если ID совпал — возвращает адрес объекта
    }
    return nullptr;                         // Если ничего не найдено — возвращает пустой указатель
}

// Назначает подразделение на инцидент с проверками корректности и логированием
bool DispatchCenter::assign(int unitId, int incidentId) {
    auto* unit = findUnit(unitId);          // Находит подразделение по ID
    auto* incident = findIncident(incidentId);  // Находит инцидент по ID

    if (!unit || !incident) {               // Проверяет, что оба объекта найдены
        log_.write("assign failed: unit or incident not found");  // Фиксирует ошибку в логе
        return false;                       // Возвращает неудачу операции
    }

    if (unit->status() != UnitStatus::Available) {  // Проверяет, доступно ли подразделение для назначения
        log_.write("assign failed: unit not available");  // Фиксирует причину отказа
        return false;
    }

    if (incident->status() != IncidentStatus::Open) {  // Проверяет, открыт ли инцидент (не закрыт и не выполнен)
        log_.write("assign failed: incident not open");  // Фиксирует причину отказа
        return false;
    }

    if (unit->zone() != incident->zone()) {  // Предупреждает, если подразделение и инцидент находятся в разных зонах
        log_.write("assign warning: unit and incident zones differ");  // Записывает предупреждение (но не блокирует назначение)
    }

    // Пытается назначить подразделение на инцидент и наоборот (двусторонняя привязка)
    if (unit->assignToIncident(incidentId) && incident->assignUnit(unitId)) {
        log_.write("assignment successful: unit " + std::to_string(unitId) +
                   " to incident " + std::to_string(incidentId));  // Фиксирует успешное назначение
        return true;
    }

    log_.write("assign failed: internal error");  // Если привязка не прошла — фиксирует внутреннюю ошибку
    return false;
}

// Закрывает инцидент с проверками и логированием
bool DispatchCenter::closeIncident(int incidentId) {
    auto* incident = findIncident(incidentId);  // Ищет инцидент по ID
    if (!incident) {                            // Проверяет, найден ли объект
        log_.write("close failed: incident not found");  // Фиксирует ошибку
        return false;
    }

    if (incident->status() == IncidentStatus::Closed) {  // Проверяет, не закрыт ли инцидент уже
        log_.write("close failed: incident already closed");  // Фиксирует повторную попытку закрытия
        return false;
    }

    incident->close();                          // Вызывает метод закрытия у объекта инцидента
    log_.write("incident closed: " + std::to_string(incidentId));  // Записывает факт закрытия в лог
    return true;
}

// Выводит в консоль список всех подразделений с их параметрами
void DispatchCenter::printUnits() const {
    std::cout << "Units:\n";                  // Печатает заголовок раздела
    for (const auto& u : units_) {            // Проходит по каждому подразделению в списке
        std::cout << "  ID: " << u.id()      // Выводит ID подразделения
                  << " | Name: " << u.name()  // Выводит имя подразделения
                  << " | Zone: " << u.zone()  // Выводит зону ответственности
                  << " | Status: " << toString(u.status())  // Выводит статус в текстовом виде
                  << " | Assigned: "          // Выводит информацию о назначенном инциденте
                  << (u.assignedIncidentId() ? std::to_string(*u.assignedIncidentId()) : "none")  // ID инцидента или «none», если нет
                  << "\n";
    }
}

// Выводит в консоль список всех инцидентов с их параметрами
void DispatchCenter::printIncidents() const {
    std::cout << "Incidents:\n";              // Печатает заголовок раздела
    for (const auto& i : incidents_) {        // Проходит по каждому инциденту в списке
        std::cout << "  ID: " << i.id()      // Выводит ID инцидента
                  << " | Title: " << i.title()  // Выводит заголовок/описание инцидента
                  << " | Zone: " << i.zone()  // Выводит зону, где произошёл инцидент
                  << " | Severity: " << toString(i.severity())  // Выводит уровень серьёзности в текстовом виде
                  << " | Status: " << toString(i.status())  // Выводит текущий статус инцидента
                  << " | Marker: " << toString(i.marker())  // Выводит визуальный маркер в текстовом виде
                  << " | Assigned: "          // Выводит информацию о назначенном подразделении
                  << (i.assignedUnitId() ? std::to_string(*i.assignedUnitId()) : "none")  // ID подразделения или «none», если нет
                  << "\n";
    }
}

// Выводит полное состояние системы: список подразделений и инцидентов
void DispatchCenter::printState() const {
    printUnits();                             // Вызывает вывод списка подразделений
    printIncidents();                         // Вызывает вывод списка инцидентов
    std::cout << "---\n";                     // Печатает разделитель для наглядности
}

} // namespace dispatcher