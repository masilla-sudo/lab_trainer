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