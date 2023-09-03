#pragma once

#include <cmath>
#include <type_traits>

#include "math.hpp"

namespace rt {

struct vec {
    float x;
    float y;
    float z;
    float w;

    constexpr vec() = default;
    constexpr vec(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

    [[nodiscard]] static constexpr vec make_point(float x, float y, float z) {
        return {x, y, z, 1.f};
    }
    [[nodiscard]] static constexpr vec make_point(float a) {
        return {a, a, a, 1.f};
    }

    [[nodiscard]] static constexpr vec make_vector(float x, float y, float z) {
        return {x, y, z, 0.f};
    }
    [[nodiscard]] static constexpr vec make_vector(float a) {
        return {a, a, a, 0.f};
    }

    [[nodiscard]] constexpr float dot(vec other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
    }

    [[nodiscard]] constexpr vec cross(vec other) const {
        return vec::make_vector(this->y * other.z - this->z * other.y,
                                this->z * other.x - this->x * other.z,
                                this->x * other.y - this->y * other.x);
    }

    [[nodiscard]] float magnitude() const {
        return std::sqrtf(std::powf(this->x, 2) + std::powf(this->y, 2) + std::powf(this->z, 2) + std::powf(this->w, 2));
    }
    [[nodiscard]] vec normalize() const {
        return *this / this->magnitude();
    }
    [[nodiscard]] bool is_unit_vector() const {
        return float_eq(this->magnitude(), 1.f);
    }

    [[nodiscard]] constexpr bool operator==(vec other) const {
        return float_eq(this->x, other.x) &&
               float_eq(this->y, other.y) &&
               float_eq(this->z, other.z) &&
               float_eq(this->w, other.w);
    }

    [[nodiscard]] constexpr vec operator+() const {
        return *this;
    }
    [[nodiscard]] constexpr vec operator-() const {
        return {-this->x, -this->y, -this->z, -this->w};
    }
    [[nodiscard]] constexpr vec operator+(vec other) const {
        return {this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w};
    }
    [[nodiscard]] constexpr vec operator-(vec other) const {
        return {this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w};
    }
    constexpr void operator+=(vec other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        this->w += other.w;
    }
    constexpr void operator-=(vec other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->w -= other.w;
    }
    [[nodiscard]] constexpr float operator*(vec other) const {
        return this->dot(other);
    }

    template<typename T>
    requires std::is_arithmetic_v<T>
    [[nodiscard]] constexpr vec operator*(T other) const {
        return {this->x * other, this->y * other, this->z * other, this->w * other};
    }
    template<typename T>
    requires std::is_arithmetic_v<T>
    [[nodiscard]] constexpr vec operator/(T other) const {
        return {this->x / other, this->y / other, this->z / other, this->w / other};
    }
    template<typename T>
    requires std::is_arithmetic_v<T>
    constexpr void operator*=(T other) {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        this->w *= other;
    }
    template<typename T>
    requires std::is_arithmetic_v<T>
    constexpr void operator/=(T other) {
        this->x /= other;
        this->y /= other;
        this->z /= other;
        this->w /= other;
    }
};

} // namespace rt
