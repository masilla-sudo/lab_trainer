#pragma once
#include <iostream>
#include <concepts>

template <typename T>
concept Streamable = requires(std::ostream& os, const T& v) {
    { os << v } -> std::same_as<std::ostream&>;
};