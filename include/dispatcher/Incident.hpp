#pragma once                              // Предотвращает многократное подключение этого заголовочного файла
#include "dispatcher/DispatchTypes.hpp"   // Подключает типы и перечисления для диспетчерской системы (статусы, уровни серьёзности, маркеры и т.п.)
#include <optional>                       // Подключает тип optional — позволяет хранить значение или отсутствие значения (например, пока не назначили подразделение)
#include <string>                         // Подключает стандартную библиотеку строк для работы с текстовыми данными

namespace dispatcher {                    // Пространство имён для компонентов диспетчерской системы

class Incident {                          // Класс, описывающий инцидент (происшествие/аварию) в диспетчерском тренажёре
public:
    Incident(int id, std::string title, std::string zone,
             Severity severity, VisualMarker marker);  // Конструктор: создаёт инцидент с ID, заголовком, зоной, уровнем серьёзности и визуальным маркером

    int id() const;                                   // Возвращает уникальный идентификатор инцидента
    const std::string& title() const;                 // Возвращает заголовок/описание инцидента (без копирования строки)
    const std::string& zone() const;                  // Возвращает зону, где произошёл инцидент
    Severity severity() const;                       // Возвращает уровень серьёзности инцидента (Low/Medium/High/Critical)
    IncidentStatus status() const;                   // Возвращает текущий статус инцидента (Open/Assigned/Closed)
    VisualMarker marker() const;                     // Возвращает визуальный маркер для отображения на схеме (Alert, Repair и т.д.)
    std::optional<int> assignedUnitId() const;       // Возвращает ID назначенного подразделения либо пустое значение, если никто не назначен

    bool assignUnit(int unitId);                     // Назначает подразделение на этот инцидент (возвращает успех операции)
    void close();                                    // Закрывает инцидент — переводит его в статус Closed

private:
    int id_{};                                        // Уникальный идентификатор инцидента
    std::string title_;                               // Заголовок/краткое описание инцидента
    std::string zone_;                                // Зона, где зафиксирован инцидент
    Severity severity_{Severity::Low};               // Уровень серьёзности (по умолчанию — низкий)
    IncidentStatus status_{IncidentStatus::Open};   // Текущий статус инцидента (по умолчанию — открыт)
    VisualMarker marker_{VisualMarker::None};       // Визуальный маркер для отображения (по умолчанию — без маркера)
    std::optional<int> assignedUnitId_;              // ID назначенного подразделения либо пустое значение (если никто не назначен)
};

} // namespace dispatcher