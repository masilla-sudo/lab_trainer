#pragma once

#include "check_result.hpp"
#include "parameter.hpp"
#include "range.hpp"

#include <memory>
#include <optional>
#include <string>

// ─── Полиморфный интерфейс правила проверки ───
// Тот же интерфейс, что и в ЛР 5, но теперь правила создаются
// через шаблоны и встраиваются в полиморфную иерархию.
class ICheckRule {
public:
    virtual ~ICheckRule() = default;
    virtual CheckResult check() const = 0;
};

// ─── Обобщённое правило диапазона ───
// RangeRule<T> — шаблонный класс, наследующий ICheckRule.
// Связывает Parameter<T> и Range<T>, производит CheckResult.
//
// Два режима:
//   1) Только normalRange  → Ok / Warning
//   2) normalRange + criticalRange → Ok / Warning / Critical
//      (доп. задание: Severity::Critical)
template <OrderedValue T>
class RangeRule final : public ICheckRule {
public:
    // Конструктор без критического диапазона.
    RangeRule(const Parameter<T>& parameter,
              Range<T> normalRange,
              std::string problemMessage)
        : parameter_(parameter)
        , normalRange_(std::move(normalRange))
        , criticalRange_(std::nullopt)
        , problemMessage_(std::move(problemMessage))
    {}

    // Конструктор с критическим (аварийным) диапазоном — доп. задание.
    // Если значение попадает в criticalRange — Critical.
    // Если в normalRange — Ok.
    // Иначе — Warning.
    RangeRule(const Parameter<T>& parameter,
              Range<T> normalRange,
              Range<T> criticalRange,
              std::string problemMessage)
        : parameter_(parameter)
        , normalRange_(std::move(normalRange))
        , criticalRange_(std::move(criticalRange))
        , problemMessage_(std::move(problemMessage))
    {}

    CheckResult check() const override
    {
        const T& v = parameter_.value();

        // Сначала проверяем критический диапазон (если задан).
        if (criticalRange_ && criticalRange_->contains(v)) {
            return {parameter_.id(), problemMessage_ + " (CRITICAL)", Severity::Critical};
        }

        // Затем — нормальный диапазон.
        if (normalRange_.contains(v)) {
            return {parameter_.id(), "OK", Severity::Ok};
        }

        return {parameter_.id(), problemMessage_, Severity::Warning};
    }

private:
    const Parameter<T>&           parameter_;
    Range<T>                      normalRange_;
    std::optional<Range<T>>       criticalRange_;
    std::string                   problemMessage_;
};