#include <dispatcher/core/check_result.hpp>
#include <dispatcher/core/parameter.hpp>
#include <dispatcher/core/range.hpp>
#include <dispatcher/core/rules.hpp>
#include <dispatcher/core/signal_state.hpp>
#include <dispatcher/core/is_inside_range.hpp>
#include <dispatcher/presentation/visual_descriptor.hpp>

#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <string>

// ─── Визуальные слои (из предыдущей работы — сохраняем преемственность) ───
const std::array<std::string, 5>& visualLayerNames() {
    static const std::array<std::string, 5> layers = {
        "platform_map",      // Карта платформ — базовый слой
        "passenger_flow",    // Поток пассажиров
        "gates",             // Выходы/гейты
        "delays",            // Задержки
        "ar_gate_marker"     // AR-маркер у гейта
    };
    return layers;
}

// Вариант: Транспортный диспетчер
// Параметры: скорость, интервал, задержка, занятость пути, состояние светофора.
int main()
{
    // ─── Создаём типизированные параметры транспортного узла ───
    // Скорость поезда — double, норма 0..80 км/ч
    Parameter<double> speed("V-101", "Скорость поезда", 65.0, "km/h");
    // Интервал между поездами — double, норма 2..10 мин
    Parameter<double> interval("I-201", "Интервал", 3.5, "min");
    // Задержка — int, норма 0..5 мин
    Parameter<int> delay("D-301", "Задержка", 7, "min");
    // Занятость пути — int, норма 0..80 %
    Parameter<int> occupancy("O-401", "Занятость пути", 45, "%");
    // Состояние светофора — SignalState (перечисление)
    Parameter<SignalState> signal("S-501", "Светофор", SignalState::On, "state");

    // ─── Проверка через функциональный шаблон isInsideRange ───
    std::cout << "=== Проверка isInsideRange (функциональный шаблон) ===\n";
    std::cout << "isInsideRange(65.0, 0.0, 80.0) = "
              << isInsideRange(65.0, 0.0, 80.0) << "\n";
    std::cout << "isInsideRange(3.5, 2.0, 10.0) = "
              << isInsideRange(3.5, 2.0, 10.0) << "\n";
    std::cout << "isInsideRange(7, 0, 5) = "
              << isInsideRange(7, 0, 5) << "\n\n";

    // ─── Создаём правила проверки ───
    // Вектор unique_ptr<ICheckRule> — полиморфизм времени выполнения,
    // но сами правила создаются через шаблоны (компиляционное обобщение).
    std::vector<std::unique_ptr<ICheckRule>> rules;

    // Скорость: норма 0..80, критика выше 100
    rules.push_back(std::make_unique<RangeRule<double>>(
        speed,
        Range<double>{0.0, 80.0},
        Range<double>{100.0, 300.0},
        "Скорость вне нормы"
    ));

    // Интервал: норма 2..10 (без критического диапазона)
    rules.push_back(std::make_unique<RangeRule<double>>(
        interval,
        Range<double>{2.0, 10.0},
        "Интервал вне нормы"
    ));

    // Задержка: норма 0..5, критика выше 15
    rules.push_back(std::make_unique<RangeRule<int>>(
        delay,
        Range<int>{0, 5},
        Range<int>{15, 999},
        "Задержка недопустима"
    ));

    // Занятость пути: норма 0..80
    rules.push_back(std::make_unique<RangeRule<int>>(
        occupancy,
        Range<int>{0, 80},
        "Занятость пути вне нормы"
    ));

    // ─── Выполняем проверки ───
    std::cout << "=== Проверка параметров через RangeRule<T> ===\n";
    std::vector<CheckResult> results;
    for (const auto& rule : rules) {
        CheckResult r = rule->check();
        results.push_back(r);
        std::cout << r.parameterId << ": " << r.message
                  << " [" << static_cast<int>(r.severity) << "]\n";
    }

    // ─── Визуальные дескрипторы (DTO для будущей визуализации) ───
    std::cout << "\n=== Визуальные дескрипторы (DTO) ===\n";
    {
        auto vd = makeDescriptor(speed, results[0]);
        std::cout << vd.objectId << " | " << vd.visualKind
                  << " | " << vd.label << " | " << vd.color
                  << " | prio=" << vd.priority << "\n";
    }
    {
        auto vd = makeDescriptor(interval, results[1]);
        std::cout << vd.objectId << " | " << vd.visualKind
                  << " | " << vd.label << " | " << vd.color
                  << " | prio=" << vd.priority << "\n";
    }
    {
        auto vd = makeDescriptor(delay, results[2]);
        std::cout << vd.objectId << " | " << vd.visualKind
                  << " | " << vd.label << " | " << vd.color
                  << " | prio=" << vd.priority << "\n";
    }
    {
        auto vd = makeDescriptor(occupancy, results[3]);
        std::cout << vd.objectId << " | " << vd.visualKind
                  << " | " << vd.label << " | " << vd.color
                  << " | prio=" << vd.priority << "\n";
    }

    // ─── Визуальные слои (из предыдущей работы) ───
    std::cout << "\n=== Визуальные слои ===\n";
    for (const auto& layer : visualLayerNames()) {
        std::cout << "- " << layer << "\n";
    }

    // ─── Состояние светофора (SignalState) ───
    std::cout << "\n=== Состояние светофора (enum class SignalState) ===\n";
    std::cout << signal.id() << " (" << signal.label() << "): "
              << toString(signal.value()) << "\n";

    std::cout << "\nГотово.\n";
    return 0;
}