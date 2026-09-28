#include "trainer/model.hpp"   // Подключает объявления типов ParameterRecord, ParameterList и других сущностей тренажёрной модели
#include <iostream>           // Нужен для вывода в консоль (std::cout)
#include <array>              // Требуется для std::array — используется в visualLayerNames()

// Возвращает список имён слоёв визуализации (статический массив, создаётся один раз)
const std::array<std::string, 5>& visualLayerNames() {
    // static гарантирует, что массив создаётся только при первом вызове и живёт до конца программы
    static const std::array<std::string, 5> layers = {
        "platform_map",        // Карта платформ — базовый слой схемы
        "passenger_flow",      // Поток пассажиров — динамика перемещения людей
        "gates",               // Выходы/гейты — точки прохода и контроля
        "delays",              // Задержки — отображение проблемных участков и опозданий
        "ar_gate_marker"       // AR‑маркер у гейта — для дополненной реальности или подсветки на схеме
    };
    return layers;             // Возвращает ссылку на массив (без копирования)
}

// Проверяет, находится ли значение параметра в допустимом диапазоне [minValue..maxValue]
bool isInRange(const ParameterRecord& parameter) {
    return parameter.value >= parameter.minValue &&
           parameter.value <= parameter.maxValue;
}

// Выводит список параметров транспортного узла в читаемом виде (для отладки и демонстрации)
void printParameters(const ParameterList& parameters) {
    std::cout << "Параметры транспортного узла:\n";
    for (const ParameterRecord& p : parameters) {
        // Для каждого параметра выводит: имя, текущее значение, единицу измерения и допустимый диапазон
        std::cout << "- " << p.name << " = " << p.value << " " << p.unit
                  << " [" << p.minValue << ".." << p.maxValue << "]\n";
    }
}

// Пытается установить новое значение для параметра по его имени
// Возвращает true, если параметр найден и значение обновлено; false — если параметра с таким именем нет
bool setParameter(ParameterList& parameters, const std::string& name, double newValue) {
    for (ParameterRecord& p : parameters) {
        if (p.name == name) {      // Ищет параметр по имени (строковое сравнение)
            p.value = newValue;     // Обновляет значение
            return true;            // Сообщает об успехе
        }
    }
    return false;                   // Сообщает, что параметр не найден
}
#include <algorithm>
#include <iterator>

// --- find_if: поиск параметра по имени ---
const ParameterRecord* findParameterByName(const ParameterList& parameters,
                                            const std::string& name) {
    auto it = std::find_if(parameters.begin(), parameters.end(),
        [&name](const ParameterRecord& p) {    // lambda #1
            return p.name == name;
        });
    return (it != parameters.end()) ? &(*it) : nullptr;
}

// --- transform: список имён параметров ---
std::vector<std::string> getParameterNames(const ParameterList& parameters) {
    std::vector<std::string> names;
    names.reserve(parameters.size());
    std::transform(parameters.begin(), parameters.end(),
                   std::back_inserter(names),
                   [](const ParameterRecord& p) {    // lambda #2
                       return p.name;
                   });
    return names;
}

// --- transform: краткие строки "name=value unit" ---
std::vector<std::string> getParameterSummary(const ParameterList& parameters) {
    std::vector<std::string> summary;
    summary.reserve(parameters.size());
    std::transform(parameters.begin(), parameters.end(),
                   std::back_inserter(summary),
                   [](const ParameterRecord& p) {    // lambda #3
                       return p.name + " = " + std::to_string(p.value) + " " + p.unit;
                   });
    return summary;
}