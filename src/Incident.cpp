#include "dispatcher/Incident.hpp"  // Подключает объявление класса Incident и всех используемых типов (Severity, VisualMarker и др.)
#include <stdexcept>                // Подключает стандартные исключения — нужно для выбрасывания std::invalid_argument при ошибках валидации

namespace dispatcher {              // Пространство имён для компонентов диспетчерской системы

// Конструктор: создаёт инцидент с проверкой корректности входных данных
Incident::Incident(int id, std::string title, std::string zone,
                   Severity severity, VisualMarker marker)
    : id_{id}, title_{std::move(title)}, zone_{std::move(zone)},
      severity_{severity}, marker_{marker} {  // Инициализирует поля; std::move экономит копирование строк
    if (id <= 0) {                            // Проверяет, что идентификатор положительный
        throw std::invalid_argument("Incident ID must be positive");  // Если ID некорректен — выбрасывает исключение
    }
    if (title_.empty()) {                      // Проверяет, что заголовок не пустой
        throw std::invalid_argument("Incident title cannot be empty");  // При отсутствии заголовка — выбрасывает исключение
    }
    if (zone_.empty()) {                       // Проверяет, что зона не пустая
        throw std::invalid_argument("Incident zone cannot be empty"); // При отсутствии зоны — выбрасывает исключение
    }
}

// Возвращает уникальный идентификатор инцидента
int Incident::id() const { return id_; }

// Возвращает заголовок/описание инцидента (через константную ссылку — без копирования строки)
const std::string& Incident::title() const { return title_; }

// Возвращает зону, где зафиксирован инцидент (через константную ссылку)
const std::string& Incident::zone() const { return zone_; }

// Возвращает уровень серьёзности инцидента (Low, Medium, High, Critical)
Severity Incident::severity() const { return severity_; }

// Возвращает текущий статус инцидента (Open, Assigned, Closed)
IncidentStatus Incident::status() const { return status_; }

// Возвращает визуальный маркер для отображения на схеме (Alert, Repair и т. д.)
VisualMarker Incident::marker() const { return marker_; }

// Возвращает ID назначенного подразделения либо пустое значение (nullopt), если никто не назначен
std::optional<int> Incident::assignedUnitId() const {
    return assignedUnitId_;
}

// Пытается назначить подразделение на инцидент: меняет статус и фиксирует ID подразделения
bool Incident::assignUnit(int unitId) {
    if (status_ != IncidentStatus::Open) {  // Проверяет, находится ли инцидент в состоянии «открыт»
        return false;                       // Если нет (уже назначен или закрыт) — отклоняет назначение
    }
    status_ = IncidentStatus::Assigned;     // Переводит статус в «назначен»
    assignedUnitId_ = unitId;               // Записывает ID назначенного подразделения
    return true;                            // Сообщает об успешном назначении
}

// Закрывает инцидент: переводит в статус Closed и сбрасывает привязку к подразделению
void Incident::close() {
    if (status_ == IncidentStatus::Closed) {  // Проверяет, не закрыт ли инцидент уже
        return;                                // Если закрыт — ничего не делает (защита от повторных действий)
    }
    status_ = IncidentStatus::Closed;         // Устанавливает статус «закрыт»
    assignedUnitId_.reset();                  // Очищает ID подразделения (делает значение пустым)
}

} // namespace dispatcher