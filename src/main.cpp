#include "dispatcher/core/check_result.hpp"
#include "dispatcher/core/parameter.hpp"
#include "dispatcher/core/range.hpp"
#include "dispatcher/core/rules.hpp"
#include "dispatcher/core/signal_state.hpp"
#include "dispatcher/core/concepts.hpp"
#include "dispatcher/presentation/visual_descriptor.hpp"

#include <iostream>
#include <memory>
#include <vector>

int main() {
    // --- Параметры железнодорожной станции ---
    Parameter<double> speed("V-101", "Скорость поезда", 55.0, "км/ч");
    Parameter<double> interval("I-201", "Интервал между поездами", 7.0, "мин");
    Parameter<int>    occupancy("O-301", "Занятость пути", 2, "пути");
    Parameter<SignalState> signal("S-501", "Светофор", SignalState::Green, "состояние");

    // --- Правила проверки ---
    std::vector<std::unique_ptr<ICheckRule>> rules;

    rules.push_back(std::make_unique<RangeRule<double>>(
        speed,
        Range<double>{0.0, 80.0},
        std::optional<Range<double>>(Range<double>{100.0, 200.0}),
        "Скорость вне нормы"
    ));

    rules.push_back(std::make_unique<RangeRule<double>>(
        interval,
        Range<double>{5.0, 10.0},
        std::nullopt,
        "Интервал вне нормы"
    ));

    rules.push_back(std::make_unique<RangeRule<int>>(
        occupancy,
        Range<int>{0, 5},
        std::optional<Range<int>>(Range<int>{10, 99}),
        "Занятость пути вне нормы"
    ));

    // --- Проверка параметров через RangeRule<T> ---
    std::cout << "=== Проверка параметров через RangeRule<T> ===\n";
    for (const auto& rule : rules) {
        CheckResult result = rule->check();

        std::cout << result.parameterId << ": " << result.message << " [";
        switch (result.severity) {
            case Severity::Ok:       std::cout << "OK"; break;
            case Severity::Warning:  std::cout << "WARNING"; break;
            case Severity::Critical: std::cout << "CRITICAL"; break;
        }
        std::cout << "]\n";
    }

    // --- Демонстрация SignalState ---
    std::cout << "\n=== Состояние сигнала ===\n";
    std::cout << "Светофор: " << signal.value() << "\n";
    signal.setValue(SignalState::Red);
    std::cout << "После переключения: " << signal.value() << "\n";

    // --- Демонстрация makeDescriptor (задание повышенной сложности) ---
    std::cout << "\n=== VisualDescriptor для каждого параметра ===\n";

    auto descSpeed = makeDescriptor(speed, rules[0]->check());
    std::cout << descSpeed.objectId << " | " << descSpeed.label
              << " | " << descSpeed.valueStr
              << " | " << descSpeed.color
              << " | " << descSpeed.visualKind << "\n";

    auto descInterval = makeDescriptor(interval, rules[1]->check());
    std::cout << descInterval.objectId << " | " << descInterval.label
              << " | " << descInterval.valueStr
              << " | " << descInterval.color
              << " | " << descInterval.visualKind << "\n";

    auto descOccupancy = makeDescriptor(occupancy, rules[2]->check());
    std::cout << descOccupancy.objectId << " | " << descOccupancy.label
              << " | " << descOccupancy.valueStr
              << " | " << descOccupancy.color
              << " | " << descOccupancy.visualKind << "\n";

    // --- Проверка Warning: меняем интервал ---
    std::cout << "\n=== Проверка Warning ===\n";
    interval.setValue(3.0);
    auto warnResult = rules[1]->check();
    auto warnDesc = makeDescriptor(interval, warnResult);
    std::cout << warnDesc.objectId << " | " << warnDesc.label
              << " | " << warnDesc.valueStr
              << " | " << warnDesc.color
              << " | " << warnDesc.visualKind << "\n";

      // --- Проверка Critical ---
    std::cout << "\n=== Проверка Critical ===\n";
    speed.setValue(140.0);
    auto critResult = rules[0]->check();
    auto critDesc = makeDescriptor(speed, critResult);
    std::cout << critDesc.objectId << " | " << critDesc.label
              << " | " << critDesc.valueStr
              << " | " << critDesc.color
              << " | " << critDesc.visualKind << "\n";
              return 0;
}