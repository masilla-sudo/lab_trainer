#pragma once

// Функциональный шаблон — семейство функций для разных типов.
// Не привязан к классу Range<T>; может использоваться самостоятельно.
// Требование лабы: минимум один функциональный шаблон.

template <typename T>
bool isInsideRange(const T& value, const T& min, const T& max)
{
    return min <= value && value <= max;
}