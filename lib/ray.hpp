#pragma once

#include "mat.hpp"

namespace rt {

struct ray {
    vec origin;
    vec direction;

    constexpr ray() = default;
    constexpr ray(vec origin_, vec direction_) : origin(origin_), direction(direction_) {}

    [[nodiscard]] constexpr float dot(ray other) const {
        return this->direction.dot(other.direction);
    }
    [[nodiscard]] constexpr vec cross(ray other) const {
        return this->direction.cross(other.direction);
    }
    [[nodiscard]] constexpr float dot(vec other) const {
        return this->direction.dot(other);
    }
    [[nodiscard]] constexpr vec cross(vec other) const {
        return this->direction.cross(other);
    }

    template<typename T>
    requires std::is_arithmetic_v<T>
    [[nodiscard]] constexpr vec point_along(T distance) const {
        return this->origin + this->direction * distance;
    }

    [[nodiscard]] constexpr ray transform(const mat<4, 4>& transformation) const {
        return {transformation * this->origin, transformation * this->direction};
    }

    [[nodiscard]] constexpr bool operator==(ray other) const {
        return this->origin == other.origin && this->direction == other.direction;
    }

    [[nodiscard]] constexpr ray operator+() const {
        return *this;
    }
    [[nodiscard]] constexpr ray operator-() const {
        return {this->origin, -this->direction};
    }

    [[nodiscard]] constexpr float operator*(ray other) const {
        return this->dot(other);
    }
    [[nodiscard]] constexpr float operator*(vec other) const {
        return this->dot(other);
    }
    [[nodiscard]] constexpr ray operator*(const mat<4, 4>& transformation) const {
        return this->transform(transformation);
    }
    constexpr void operator*=(const mat<4, 4>& transformation) {
        *this = this->transform(transformation);
    }
};

} // namespace rt
