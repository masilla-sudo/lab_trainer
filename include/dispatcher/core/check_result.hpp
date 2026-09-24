#pragma once

#include <string>

// Уровень серьёзности результата проверки.
// Ok       — параметр в норме
// Warning  — параметр вышел за нормальный диапазон
// Critical — параметр достиг аварийного значения
enum class Severity {
    Ok,
    Warning,
    Critical
};

// Результат проверки — не зависит от типа параметра.
// Один и тот же CheckResult работает и для double, и для int,
// и для будущих перечислимых типов.
struct CheckResult {
    std::string parameterId;          // идентификатор параметра (P-101, T-201, ...)
    std::string message;              // человекочитаемое сообщение
    Severity    severity = Severity::Ok;
};