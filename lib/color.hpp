#pragma once

#include <type_traits>

#include "math.hpp"

namespace rt {

struct color {
    float r;
    float g;
    float b;

    constexpr color() = default;
    constexpr color(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}

    [[nodiscard]] constexpr bool operator==(color other) const {
        return float_eq(this->r, other.r) &&
               float_eq(this->g, other.g) &&
               float_eq(this->b, other.b);
    }
    [[nodiscard]] constexpr bool operator!=(color other) const {
        return !(*this == other);
    }

    [[nodiscard]] constexpr color operator+(color other) const {
        return {this->r + other.r, this->g + other.g, this->b + other.b};
    }
    [[nodiscard]] constexpr color operator-(color other) const {
        return {this->r - other.r, this->g - other.g, this->b - other.b};
    }
    [[nodiscard]] constexpr color operator*(color other) const {
        return {this->r * other.r, this->g * other.g, this->b * other.b};
    }

    template<typename T>
    requires std::is_arithmetic_v<T>
    [[nodiscard]] constexpr color operator*(T other) const {
        return {this->r * other, this->g * other, this->b * other};
    }
    template<typename T>
    requires std::is_arithmetic_v<T>
    [[nodiscard]] constexpr color operator/(T other) const {
        return {this->r / other, this->g / other, this->b / other};
    }
};

} // namespace rt
