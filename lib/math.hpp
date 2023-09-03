#pragma once

#include <limits>
#include <type_traits>

namespace rt {

constexpr float PI = 3.141592;
constexpr float PI_2 = PI / 2;
constexpr float PI_4 = PI / 4;

template<typename T>
requires std::is_arithmetic_v<T>
[[nodiscard]] constexpr T abs_constexpr(T value) {
    return value > 0 ? value : -value;
}

template<typename T>
requires std::is_floating_point_v<T>
constexpr bool float_eq(T a, T b, T epsilon = 0.00001f) {
    return abs_constexpr(a - b) < epsilon;
}

} // namespace rt
