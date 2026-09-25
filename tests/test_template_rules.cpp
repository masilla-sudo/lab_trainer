#include "dispatcher/core/parameter.hpp"
#include "dispatcher/core/range.hpp"
#include "dispatcher/core/rules.hpp"
#include <cassert>

int main() {
    // Тест Range<int>
    Range<int> intRange{1, 10};
    assert(intRange.contains(1));
    assert(intRange.contains(5));
    assert(!intRange.contains(11));

    // Тест Range<double>
    Range<double> dblRange{6.0, 8.0};
    assert(dblRange.contains(7.5));
    assert(!dblRange.contains(8.1));

    // Тест RangeRule — значение в норме
    Parameter<double> p("P", "Pressure", 7.5, "MPa");
    RangeRule<double> rule(p, Range<double>{6.0, 8.0},
                          std::optional<Range<double>>(Range<double>{10.0, 20.0}),
                          "bad");
    assert(rule.check().severity == Severity::Ok);

    // Тест Warning — значение вне нормы
    p.setValue(9.2);
    assert(rule.check().severity == Severity::Warning);

    // Тест Critical — значение в критическом диапазоне
    p.setValue(15.0);
    assert(rule.check().severity == Severity::Critical);

    // Тест без критического диапазона (std::nullopt)
    Parameter<int> level("L", "Level", 50, "%");
    RangeRule<int> levelRule(level, Range<int>{20, 90},
                             std::nullopt, "level bad");
    assert(levelRule.check().severity == Severity::Ok);
    level.setValue(95);
    assert(levelRule.check().severity == Severity::Warning);

    return 0;
}