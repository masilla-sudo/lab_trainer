#pragma once

#include <concepts>
#include <stdexcept>

// concept: тип должен поддерживать сравнение <= и >=,
// результат которых приводится к bool.
// Если передать тип без этих операций, компилятор выдаст
// понятное сообщение: "constraints not satisfied", а не стену ошибок.
template <typename T>
concept OrderedValue = requires(T a, T b) {
    { a <= b } -> std::convertible_to<bool>;
    { a >= b } -> std::convertible_to<bool>;
};

// Обобщённый диапазон допустимых значений.
// Работает для double, int, float и любых типов,
// удовлетворяющих concept OrderedValue.
template <OrderedValue T>
struct Range {
    T min{};
    T max{};

    // Конструктор проверяет, что min <= max.
    // Если нет — бросаем исключение, чтобы ошибка
    // обнаружилась на этапе создания, а не при проверке.
    Range(T minValue, T maxValue)
        : min(minValue), max(maxValue)
    {
        if (!(min <= max)) {
            throw std::invalid_argument("Range: min must be <= max");
        }
    }

    // Попадает ли значение в диапазон [min, max]?
    bool contains(const T& value) const
    {
        return min <= value && value <= max;
    }
};