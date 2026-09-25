#pragma once
#include <concepts>
#include <stdexcept>

template <typename T>
concept OrderedValue = requires(T a, T b) {
    { a <= b } -> std::convertible_to<bool>;
    { a >= b } -> std::convertible_to<bool>;
};

template <OrderedValue T>
struct Range {
    T min{};
    T max{};

    Range(T minValue, T maxValue)
        : min(minValue), max(maxValue)
    {
        if (!(min <= max)) {
            throw std::invalid_argument("Range min must be <= max");
        }
    }

    bool contains(const T& value) const {
        return min <= value && value <= max;
    }
};