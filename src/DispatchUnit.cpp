#include "dispatcher/DispatchUnit.hpp"  // Подключает объявление класса DispatchUnit и всех используемых типов (включая UnitStatus и др.)
#include <stdexcept>                    // Подключает стандартные исключения — нужно для выбрасывания std::invalid_argument при ошибках ввода

namespace dispatcher {                  // Пространство имён для компонентов диспетчерской системы

// Конструктор: создаёт объект подразделения с валидацией входных данных
DispatchUnit::DispatchUnit(int id, std::string name, std::string zone)
    : id_{id}, name_{std::move(name)}, zone_{std::move(zone)} {  // Инициализирует поля; std::move экономит копирование строк
    if (id <= 0) {                                                 // Проверяет, что идентификатор положительный
        throw std::invalid_argument("Unit ID must be positive");  // Если ID некорректен — выбрасывает исключение
    }
    if (name_.empty()) {                                           // Проверяет, что имя не пустое
        throw std::invalid_argument("Unit name cannot be empty");  // При отсутствии имени — выбрасывает исключение
    }
    if (zone_.empty()) {                                          // Проверяет, что зона не пустая
        throw std::invalid_argument("Unit zone cannot be empty"); // При отсутствии зоны — выбрасывает исключение
    }
}

// Возвращает уникальный идентификатор подразделения
int DispatchUnit::id() const { return id_; }

// Возвращает имя подразделения (через константную ссылку — без копирования строки)
const std::string& DispatchUnit::name() const { return name_; }

// Возвращает зону ответственности подразделения (через константную ссылку)
const std::string& DispatchUnit::zone() const { return zone_; }

// Возвращает текущий статус подразделения (например, Available, Busy, Maintenance, Offline)
UnitStatus DispatchUnit::status() const { return status_; }

// Возвращает ID назначенного инцидента либо пустое значение (nullopt), если подразделение ни к чему не привязано
std::optional<int> DispatchUnit::assignedIncidentId() const {
    return assignedIncidentId_;
}

// Пытается назначить подразделение на инцидент: меняет статус и фиксирует ID инцидента
bool DispatchUnit::assignToIncident(int incidentId) {
    if (status_ != UnitStatus::Available) {  // Проверяет, находится ли подразделение в состоянии «доступно»
        return false;                        // Если нет (например, уже занято или на обслуживании) — отклоняет назначение
    }
    status_ = UnitStatus::Busy;             // Переводит статус в «занято»
    assignedIncidentId_ = incidentId;       // Записывает ID назначенного инцидента
    return true;                             // Сообщает об успешном назначении
}

// Освобождает подразделение: возвращает в состояние «доступно» и снимает привязку к инциденту
void DispatchUnit::release() {
    status_ = UnitStatus::Available;        // Устанавливает статус «доступно» для новых назначений
    assignedIncidentId_.reset();            // Очищает ID инцидента (делает значение пустым)
}

// Переводит подразделение в режим техобслуживания: статус «Maintenance», снимает привязку
void DispatchUnit::setMaintenance() {
    status_ = UnitStatus::Maintenance;      // Устанавливает статус «на техобслуживании»
    assignedIncidentId_.reset();            // Сбрасывает текущее назначение (если было)
}

// Переводит подразделение в офлайн‑режим: статус «Offline», снимает привязку
void DispatchUnit::setOffline() {
    status_ = UnitStatus::Offline;          // Устанавливает статус «недоступно»
    assignedIncidentId_.reset();            // Сбрасывает текущее назначение (если было)
}

} // namespace dispatcher