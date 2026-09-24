#include <dispatcher/core/parameter.hpp>
#include <dispatcher/core/range.hpp>
#include <dispatcher/core/rules.hpp>
#include <dispatcher/core/is_inside_range.hpp>
#include <dispatcher/core/signal_state.hpp>
#include <dispatcher/presentation/visual_descriptor.hpp>

#include <cassert>
#include <iostream>
#include <stdexcept>

int main()
{
    Range<int> intRange{1, 10};
    assert(intRange.contains(1));
    assert(intRange.contains(5));
    assert(intRange.contains(10));
    assert(!intRange.contains(0));
    assert(!intRange.contains(11));

    Range<double> dblRange{6.0, 8.0};
    assert(dblRange.contains(7.0));
    assert(dblRange.contains(6.0));
    assert(dblRange.contains(8.0));
    assert(!dblRange.contains(5.9));
    assert(!dblRange.contains(8.1));

    assert(isInsideRange(5, 1, 10));
    assert(isInsideRange(7.5, 6.0, 8.0));
    assert(!isInsideRange(11, 1, 10));

    Parameter<double> p("P", "Pressure", 7.5, "MPa");
    assert(p.id() == "P");
    assert(p.label() == "Pressure");
    assert(p.unit() == "MPa");
    assert(p.value() == 7.5);

    RangeRule<double> rule(p, Range<double>{6.0, 8.0}, "bad");
    assert(rule.check().severity == Severity::Ok);

    p.setValue(9.2);
    assert(rule.check().severity == Severity::Warning);
    assert(rule.check().message == "bad");

    RangeRule<double> ruleCrit(
        p,
        Range<double>{6.0, 8.0},
        Range<double>{10.0, 100.0},
        "bad"
    );
    p.setValue(7.5);
    assert(ruleCrit.check().severity == Severity::Ok);

    p.setValue(9.2);
    assert(ruleCrit.check().severity == Severity::Warning);

    p.setValue(11.0);
    assert(ruleCrit.check().severity == Severity::Critical);

    Parameter<int> level("L", "Level", 50, "%");
    RangeRule<int> levelRule(level, Range<int>{20, 90}, "level bad");
    assert(levelRule.check().severity == Severity::Ok);

    level.setValue(95);
    assert(levelRule.check().severity == Severity::Warning);

    assert(toString(SignalState::Off) == "OFF");
    assert(toString(SignalState::On) == "ON");
    assert(toString(SignalState::Fault) == "FAULT");

    bool thrown = false;
    try {
        Range<int> bad{10, 1};
        (void)bad;
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "All tests passed!\n";
    return 0;
}
