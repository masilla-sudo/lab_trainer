#pragma once

#include <string>
#include <utility>

// Обобщённый параметр тренажера.
// T — тип значения (double для давления, int для уровня и т.д.)
// Метаданные (id, label, unit) всегда хранятся как std::string.
template <typename T>
class Parameter {
public:
    Parameter(std::string id,
              std::string label,
              T value,
              std::string unit)
        : id_(std::move(id))
        , label_(std::move(label))
        , value_(std::move(value))
        , unit_(std::move(unit))
    {}

    // Геттеры — возвращают const&, чтобы избежать лишнего копирования.
    const std::string& id()    const { return id_; }
    const std::string& label() const { return label_; }
    const std::string& unit()  const { return unit_; }
    const T&           value() const { return value_; }

    // Сеттер — используется при изменении значения параметра.
    void setValue(T value) { value_ = std::move(value); }

private:
    std::string id_;      // уникальный идентификатор
    std::string label_;   // человекочитаемое название
    T           value_{}; // текущее значение
    std::string unit_;    // единица измерения
};