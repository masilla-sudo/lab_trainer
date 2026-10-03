// ЛР 12: минимальное модульное тестирование ядра dispatcher_core.
// Подход xUnit на минимальном уровне: независимые функции test_...,
// Arrange / Act / Assert внутри каждой, общий check без внешних фреймворков.

#include <cstdlib>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

#include "trainer/model.hpp"
#include "trainer/log.hpp"

#include "dispatcher/core/check_result.hpp"
#include "dispatcher/core/is_inside_range.hpp"
#include "dispatcher/core/parameter.hpp"
#include "dispatcher/core/range.hpp"
#include "dispatcher/core/rules.hpp"

void check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << "\n";
        std::exit(1);
    }
    std::cout << "OK: " << message << "\n";
}

// --- Группа 1. Диапазоны ---

// Позитивные граничные: 0.0 и 80.0 включены (contains: min <= v && v <= max)
void test_range_contains_boundary_values() {
    const Range<double> r(0.0, 80.0);
    check(r.contains(0.0), "lower boundary 0.0 should be in range");
    check(r.contains(80.0), "upper boundary 80.0 should be in range");
    check(r.contains(60.0), "value inside range should be accepted");
    check(r.contains(79.9), "value just below upper boundary should be in range");
    check(r.contains(0.1), "value just above lower boundary should be in range");
}

// Негативные: за границами диапазона
void test_range_rejects_out_of_bounds() {
    const Range<double> r(0.0, 80.0);
    check(!r.contains(-0.1), "value below minimum should be rejected");
    check(!r.contains(80.1), "value above maximum should be rejected");
    check(!r.contains(100.0), "value far above maximum should be rejected");
}

// Негативный: конструктор с min > max должен бросить исключение
void test_range_invalid_bounds_throws() {
    bool caught = false;
    try {
        Range<double> bad(80.0, 0.0);
        (void)bad;
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    check(caught, "Range with min > max should throw std::invalid_argument");
}

// Функциональный шаблон isInsideRange — те же граничные проверки
void test_is_inside_range_template() {
    check(isInsideRange(60.0, 0.0, 80.0), "isInsideRange: normal value accepted");
    check(isInsideRange(0.0, 0.0, 80.0), "isInsideRange: lower boundary accepted");
    check(isInsideRange(80.0, 0.0, 80.0), "isInsideRange: upper boundary accepted");
    check(!isInsideRange(80.1, 0.0, 80.0), "isInsideRange: above max rejected");
    check(!isInsideRange(-0.1, 0.0, 80.0), "isInsideRange: below min rejected");
}

// Проверка isInRange для ParameterRecord (используется командой set)
void test_parameter_record_in_range() {
    const ParameterRecord inside{"V-101", 60.0, 0.0, 80.0, "км/ч"};
    const ParameterRecord outside{"V-101", 100.0, 0.0, 80.0, "км/ч"};
    check(isInRange(inside), "record inside range should pass");
    check(!isInRange(outside), "record outside range should fail");
}

// --- RangeRule: пороговая логика ЛР 8 ---
// normalRange [0..80], criticalRange [78.4..80] (2% от верхней границы)

void test_range_rule_ok_and_critical() {
    Parameter<double> p("V-101", "Скорость", 60.0, "км/ч");
    const Range<double> normal(0.0, 80.0);
    const std::optional<Range<double>> critical(Range<double>(78.4, 80.0));
    const RangeRule<double> rule(p, normal, critical,
                                 "значение в критической близости границы");

    // Arrange уже готов; Act — check(); Assert — severity и id
    auto r = rule.check();
    check(r.severity == Severity::Ok, "value 60.0 in normal range should be Ok");
    check(r.parameterId == "V-101", "result should reference parameter id");

    p.setValue(79.0);
    r = rule.check();
    check(r.severity == Severity::Critical,
          "value 79.0 in critical zone should be Critical");

    p.setValue(100.0);
    r = rule.check();
    check(r.severity == Severity::Warning,
          "value outside normal range should be Warning");
}

// --- Группа 2. Изменение состояния ---

void test_set_parameter_changes_state() {
    ParameterList parameters{{"V-101", 60.0, 0.0, 80.0, "км/ч"}};
    const bool changed = setParameter(parameters, "V-101", 70.0);
    check(changed, "set of existing parameter should succeed");
    check(parameters[0].value == 70.0, "parameter value should change to 70.0");
}

void test_set_parameter_missing_name_fails() {
    ParameterList parameters{{"V-101", 60.0, 0.0, 80.0, "км/ч"}};
    check(!setParameter(parameters, "NOPE", 10.0),
          "set of missing parameter should fail");
}

// setParameter не проверяет диапазон — меняет значение любого существующего
// параметра; валидация диапазона инкапсулирована в applyCommand.
void test_set_parameter_accepts_any_value() {
    ParameterList parameters{{"V-101", 60.0, 0.0, 80.0, "км/ч"}};
    const bool changed = setParameter(parameters, "V-101", 100.0);
    check(changed, "setParameter should change existing parameter");
    check(parameters[0].value == 100.0,
          "parameter value should change to 100.0");

    const bool roundTrip = setParameter(parameters, "V-101", 60.0);
    check(roundTrip && parameters[0].value == 60.0,
          "setParameter should restore original value");
}

// applyCommand: variant-результат команды
void test_apply_command_ok_and_error() {
    ParameterList parameters{{"V-101", 60.0, 0.0, 80.0, "км/ч"}};
    EventLog log;

    auto ok = applyCommand(parameters, log, "V-101", 70.0);
    check(std::holds_alternative<CommandOk>(ok),
          "in-range command should return CommandOk");
    check(parameters[0].value == 70.0,
          "successful command should update parameter");

   // Вне диапазона: значение применяется, но команда возвращает CommandWarning
    auto bad = applyCommand(parameters, log, "V-101", 100.0);
    check(std::holds_alternative<CommandWarning>(bad),
          "out-of-range command should return CommandWarning");
    check(parameters[0].value == 100.0,
          "applyCommand applies value even when out of range");
    check(!log.empty(),
          "out-of-range command should add an event to the log");
}

// --- Группа 3. Поиск параметра (std::optional) ---

void test_find_existing_parameter() {
    ParameterList parameters{{"V-101", 60.0, 0.0, 80.0, "км/ч"},
                             {"T-2", 5.0, 0.0, 10.0, "атм"}};
    auto first = findParameterIndex(parameters, "V-101");
    check(first.has_value(), "existing parameter should be found");
    check(*first == 0, "V-101 should be found at index 0");
    check(*findParameterIndex(parameters, "T-2") == 1,
          "T-2 should be found at index 1");
}

void test_find_missing_parameter() {
    const ParameterList parameters{{"V-101", 60.0, 0.0, 80.0, "км/ч"}};
    check(!findParameterIndex(parameters, "NOPE").has_value(),
          "missing parameter should return empty optional");
    check(!findParameterIndex(parameters, "").has_value(),
          "empty name should return empty optional");
}

// --- Группа 4. Добавление события в журнал ---

void test_add_event_increases_log() {
    EventLog log;
    check(log.empty(), "new log should be empty"); // граничный: пустой журнал

    addEvent(log, "Warning", "V-101", "близко к границе");
    check(log.size() == 1, "log size should increase by one");
    check(log.back().message == "близко к границе",
          "last event message should match added event");
    check(log.back().severity == "Warning",
          "last event severity should match added event");

    addEvent(log, "Critical", "V-102", "критическое значение");
    check(log.size() == 2, "second event should increase log to 2");
}

void test_event_statistics_and_sorted_log() {
    EventLog log;
    addEvent(log, "Warning", "V-101", "первое");
    addEvent(log, "Warning", "V-102", "второе");
    addEvent(log, "Critical", "V-103", "третье");

    const auto stats = getEventStatistics(log);
    check(stats.size() == 2, "statistics should group by two severity types");
    check(stats.at("Warning") == 2, "Warning count should be 2");
    check(stats.at("Critical") == 1, "Critical count should be 1");

    const auto sorted = getSortedEventLog(log);
    check(sorted.size() == log.size(),
          "sorted log should contain all events");
}

// --- ЛР 13: регрессия на дефект printEventLog (чтение за границей) ---
// Дефект проявлялся, когда size == capacity: цикл i <= size() читал
// элемент за границей выделенного блока (heap-buffer-overflow под ASan).
void test_print_event_log_at_full_capacity() {
    // Arrange: заполняем журнал ровно до совпадения size и capacity —
    // именно на этой границе дефеvoid test_print_event_log_at_full_capacity() {
    EventLog log;
    
    // 1. Резервируем память под 100 элементов. 
    // Теперь log.capacity() == 100 и больше не изменится при добавлении.
    const size_t target_capacity = 100;
    log.reserve(target_capacity);

    // 2. Заполняем ровно до capacity (100 элементов)
    for (size_t i = 0; i < target_capacity; ++i) {
        addEvent(log, "Info", "test", "event #" + std::to_string(i));
    }

    // 3. Проверяем, что мы действительно заполнили вектор
    check(log.size() == log.capacity(), "log should be at full capacity");
    check(log.size() == target_capacity, "log size should match reserved capacity");

    // 4. Вызываем функцию, которую мы чинили. 
    // Она должна отработать быстро и без ошибок доступа к памяти.
    printEventLog(log);
    
    check(true, "printEventLog at size == capacity should not read out of bounds");
}


int main() {
    // Группа 1: диапазоны
    test_range_contains_boundary_values();
    test_range_rejects_out_of_bounds();
    test_range_invalid_bounds_throws();
    test_is_inside_range_template();
    test_parameter_record_in_range();

    // RangeRule: пороговая логика
    test_range_rule_ok_and_critical();

    // Группа 2: изменение состояния
    test_set_parameter_changes_state();
    test_set_parameter_missing_name_fails();
    test_set_parameter_accepts_any_value();
    test_apply_command_ok_and_error();

    // Группа 3: поиск параметра
    test_find_existing_parameter();
    test_find_missing_parameter();

   // Группа 4: журнал событий
    test_add_event_increases_log();
    test_event_statistics_and_sorted_log();
    test_print_event_log_at_full_capacity(); // ЛР 13: регрессия на дефект печати журнала

    std::cout << "All tests passed\n";
    return 0;
}
