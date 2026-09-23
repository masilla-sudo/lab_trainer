#pragma once                              // Предотвращает многократное подключение этого заголовочного файла
#include <string_view>                    // Подключает тип string_view для эффективной работы со строками без копирования

namespace dispatcher {                    // Пространство имён для компонентов диспетчерской системы

enum class UnitStatus {                   // Статус диспетчерского подразделения
    Available,                           // Доступно для назначения
    Busy,                                 // Занято (обрабатывает инцидент)
    Maintenance,                          // На техобслуживании
    Offline                               // Недоступно (отключено)
};

enum class IncidentStatus {              // Статус инцидента (происшествия)
    Open,                                 // Открыт, ожидает обработки
    Assigned,                             // Назначен на подразделение
    Closed                                // Закрыт (устранён)
};

enum class Severity {                     // Уровень серьёзности инцидента
    Low,                                  // Низкий уровень угрозы
    Medium,                               // Средний уровень угрозы
    High,                                 // Высокий уровень угрозы
    Critical                              // Критический уровень угрозы
};

enum class VisualMarker {                 // Визуальный маркер для отображения на схеме/карте
    None,                                 // Без маркера
    Alert,                                // Тревога/предупреждение
    Route,                                // Маршрут движения
    Repair,                               // Требуется ремонт
    Evacuation,                           // Необходима эвакуация
    Sensor                                // Зона контроля датчиком
};

std::string_view toString(UnitStatus status);      // Возвращает строковое представление статуса подразделения
std::string_view toString(IncidentStatus status);  // Возвращает строковое представление статуса инцидента
std::string_view toString(Severity severity);      // Возвращает строковое представление уровня серьёзности
std::string_view toString(VisualMarker marker);     // Возвращает строковое представление визуального маркера

} // namespace dispatcher