#include "dispatcher/DispatchTypes.hpp"  // Подключает определения перечислений (UnitStatus, IncidentStatus и др.) для конвертации в текст
#include <string>                       // Подключает стандартную библиотеку строк — нужна для работы со string_view и строковыми литералами

namespace dispatcher {                   // Пространство имён для компонентов диспетчерской системы

// Преобразует статус подразделения в текстовое представление для вывода и логирования
std::string_view toString(UnitStatus status) {
    switch (status) {                   // Проверяет значение перечисления
        case UnitStatus::Available:   return "Available";   // Доступно для назначения
        case UnitStatus::Busy:        return "Busy";       // Занято — обрабатывает инцидент
        case UnitStatus::Maintenance: return "Maintenance";// На техобслуживании
        case UnitStatus::Offline:     return "Offline";    // Недоступно — отключено или потеряна связь
        default:                      return "Unknown";     // Неизвестный статус (защита от некорректных значений)
    }
}

// Преобразует статус инцидента в текстовое представление
std::string_view toString(IncidentStatus status) {
    switch (status) {                   // Проверяет текущий статус инцидента
        case IncidentStatus::Open:    return "Open";       // Открыт — ожидает назначения
        case IncidentStatus::Assigned:return "Assigned";    // Назначен — за ним закреплено подразделение
        case IncidentStatus::Closed:  return "Closed";     // Закрыт — проблема устранена
        default:                      return "Unknown";    // Неизвестный статус
    }
}

// Преобразует уровень серьёзности в текстовое представление — важно для приоритизации и отображения
std::string_view toString(Severity severity) {
    switch (severity) {                 // Проверяет уровень угрозы инцидента
        case Severity::Low:      return "Low";      // Низкая серьёзность — не критично
        case Severity::Medium:   return "Medium";   // Средняя серьёзность — требует внимания
        case Severity::High:     return "High";     // Высокая серьёзность — срочное реагирование
        case Severity::Critical: return "Critical"; // Критическая серьёзность — авария, максимальные ресурсы
        default:                 return "Unknown";  // Неизвестный уровень
    }
}

// Преобразует визуальный маркер в текстовое представление — нужно для отладки и вывода на консоль
std::string_view toString(VisualMarker marker) {
    switch (marker) {                   // Определяет тип визуального обозначения на схеме
        case VisualMarker::None:        return "None";        // Без маркера — обычный объект
        case VisualMarker::Alert:       return "Alert";       // Сигнал тревоги/предупреждения
        case VisualMarker::Route:       return "Route";       // Отображает маршрут движения
        case VisualMarker::Repair:      return "Repair";      // Точка, где требуется ремонт
        case VisualMarker::Evacuation:  return "Evacuation";  // Зона, требующая эвакуации
        case VisualMarker::Sensor:      return "Sensor";      // Объект под контролем датчика
        default:                        return "Unknown";      // Неизвестный маркер
    }
}

} // namespace dispatcher