#pragma once
#include <string>
#include <utility>

template <typename T>
class Parameter {
public:
    Parameter(std::string id, std::string label, T value, std::string unit)
        : id_(std::move(id)),
          label_(std::move(label)),
          value_(std::move(value)),
          unit_(std::move(unit))
    {}

    const std::string& id() const { return id_; }
    const std::string& label() const { return label_; }
    const std::string& unit() const { return unit_; }
    const T& value() const { return value_; }
    void setValue(T value) { value_ = std::move(value); }

private:
    std::string id_;
    std::string label_;
    T value_{};
    std::string unit_;
};