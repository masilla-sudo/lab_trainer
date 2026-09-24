#pragma once

#include <string>
#include <sstream>
#include <ostream>

#include "../core/check_result.hpp"
#include "../core/parameter.hpp"

// ─── DTO для будущей визуализации / AR ───
// Не зависит от GUI-библиотек. Просто описывает, как
// будущий клиент (браузер, мобильное приложение, AR-слой)
// должен отобразить параметр.
struct VisualDescriptor {
    std::string objectId;      // идентификатор объекта (P-101, T-201, ...)
    std::string visualKind;     // "gauge", "badge", "route_hint", "ar_marker"
    std::string label;         // подпись для пользователя
    std::string color;         // "green", "yellow", "red", "blue"
    int         priority = 0;  // приоритет отображения (0 — низший)
};

// Цвет по уровню серьёзности.
inline std::string colorBySeverity(Severity severity)
{
    switch (severity) {
        case Severity::Ok:       return "green";
        case Severity::Warning:  return "yellow";
        case Severity::Critical:  return "red";
    }
    return "blue";
}

// ─── concept Streamable: тип можно вывести в ostream ───
// Доп. задание: concept для типов, поддерживающих operator<<.
template <typename T>
concept Streamable = requires(std::ostream& os, const T& v) {
    { os << v } -> std::convertible_to<std::ostream&>;
};

// ─── Шаблонная функция makeDescriptor ───
// Доп. задание: формирует VisualDescriptor из Parameter<T> и CheckResult.
// Ограничена concept Streamable — значение параметра должно
// выводиться в поток, чтобы его можно было включить в метку.
template <Streamable T>
VisualDescriptor makeDescriptor(const Parameter<T>& param,
                                const CheckResult& result)
{
    std::string kind = "gauge";
    if (param.unit() == "%" || param.unit() == "state") {
        kind = "badge";
    }

    std::string label = param.label() + ": ";
    // concept Streamable гарантирует, что operator<< определён.
    std::ostringstream oss;
    oss << param.value();
    label += oss.str() + " " + param.unit();

    return {
        param.id(),
        kind,
        label,
        colorBySeverity(result.severity),
        static_cast<int>(result.severity)  // Ok=0, Warning=1, Critical=2
    };
}