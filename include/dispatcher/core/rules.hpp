#pragma once
#include "check_result.hpp"
#include "parameter.hpp"
#include "range.hpp"
#include <memory>
#include <optional>
#include <string>
#include <utility>

class ICheckRule {
public:
    virtual ~ICheckRule() = default;
    virtual CheckResult check() const = 0;
};

template <OrderedValue T>
class RangeRule final : public ICheckRule {
public:
    RangeRule(const Parameter<T>& parameter,
              Range<T> normalRange,
              std::optional<Range<T>> criticalRange,
              std::string problemMessage)
        : parameter_(parameter),
          normalRange_(normalRange),
          criticalRange_(std::move(criticalRange)),
          problemMessage_(std::move(problemMessage))
    {}

    CheckResult check() const override {
        const T& v = parameter_.value();

        if (criticalRange_.has_value() && criticalRange_->contains(v)) {
            return {parameter_.id(), problemMessage_, Severity::Critical};
        }

        if (normalRange_.contains(v)) {
            return {parameter_.id(), "OK", Severity::Ok};
        }

        return {parameter_.id(), problemMessage_, Severity::Warning};
    }

private:
    const Parameter<T>& parameter_;
    Range<T> normalRange_;
    std::optional<Range<T>> criticalRange_;
    std::string problemMessage_;
};
