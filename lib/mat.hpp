#pragma once

#include <array>
#include <cmath>

#include "math.hpp"
#include "vec.hpp"

namespace rt {

template<int W, int H>
struct mat {
    template<typename... Values>
    explicit constexpr mat(Values... vals) {
        static_assert(W >= 1 && H >= 1);
        int i = 0;
        ((values[i++] = static_cast<float>(vals)), ...);
    }

    [[nodiscard]] static constexpr mat<W, H> make_identity() {
        static_assert(W == H, "Identity matrices must be square!");
        mat<W, H> out;
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                if (i == j) {
                    out(i, j) = 1.f;
                } else {
                    out(i, j) = 0.f;
                }
            }
        }
        return out;
    }

    [[nodiscard]] static constexpr mat<4, 4> make_translation(float x, float y, float z) {
        return mat<4, 4>{
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1
        };
    }
    [[nodiscard]] static constexpr mat<4, 4> make_translation(vec point) {
        return mat<4, 4>::make_translation(point.x, point.y, point.z);
    }
    [[nodiscard]] static constexpr mat<4, 4> make_translation(float a) {
        return mat<4, 4>::make_translation(a, a, a);
    }

    [[nodiscard]] static constexpr mat<4, 4> make_scaled(float x, float y, float z) {
        return mat<4, 4>{
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1
        };
    }
    [[nodiscard]] static constexpr mat<4, 4> make_scaled(vec point) {
        return mat<4, 4>::make_scaled(point.x, point.y, point.z);
    }
    [[nodiscard]] static constexpr mat<4, 4> make_scaled(float a) {
        return mat<4, 4>::make_scaled(a, a, a);
    }

    [[nodiscard]] static constexpr mat<4, 4> make_rotated_x(float x) {
        return mat<4, 4>{
            1, 0, 0, 0,
            0, std::cos(x), -std::sin(x), 0,
            0, std::sin(x), std::cos(x), 0,
            0, 0, 0, 1
        };
    }

    [[nodiscard]] static constexpr mat<4, 4> make_rotated_y(float y) {
        return mat<4, 4>{
            std::cos(y), 0, std::sin(y), 0,
            0, 1, 0, 0,
            -std::sin(y), 0, std::cos(y), 0,
            0, 0, 0, 1
        };
    }

    [[nodiscard]] static constexpr mat<4, 4> make_rotated_z(float z) {
        return mat<4, 4>{
            std::cos(z), -std::sin(z), 0, 0,
            std::sin(z), std::cos(z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    [[nodiscard]] static constexpr mat<4, 4> make_sheared(float xy, float xz, float yx, float yz, float zx, float zy) {
        return mat<4, 4>{
            1, xy, xz, 0,
            yx, 1, yz, 0,
            zx, zy, 1, 0,
            0, 0, 0, 1
        };
    }

    [[nodiscard]] constexpr mat<H, W> transpose() const {
        mat<H, W> out;
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                out(i, j) = this->get(j, i);
            }
        }
        return out;
    }

    [[nodiscard]] constexpr mat<W - 1, H - 1> sub(int row, int col) const {
        mat<W - 1, H - 1> out;
        bool hitRow = false;
        for (int i = 0; i < W; i++) {
            if (i == row) {
                hitRow = true;
                continue;
            }
            bool hitCol = false;
            for (int j = 0; j < H; j++) {
                if (j == col) {
                    hitCol = true;
                    continue;
                }
                out(hitRow ? i - 1 : i, hitCol ? j - 1 : j) = this->get(i, j);
            }
        }
        return out;
    }

    [[nodiscard]] constexpr float minor(int row, int col) const {
        static_assert(W == H);
        if constexpr (W == 1) {
            return this->get(0, 0);
        } else if constexpr (W == 2) {
            return this->get(row, col);
        } else {
            auto m = this->sub(row, col);
            return m.determinant();
        }
    }

    [[nodiscard]] constexpr float cofactor(int row, int col) const {
        return this->minor(row, col) * ((row + col) % 2 == 0 ? 1 : -1);
    }

    [[nodiscard]] constexpr float determinant() const {
        static_assert(W == H);
        if constexpr (W == 1) {
            return this->get(0, 0);
        } else if constexpr (W == 2) {
            return this->get(0, 0) * this->get(1, 1) - this->get(1, 0) * this->get(0, 1);
        } else {
            float out = 0.f;
            for (int i = 0; i < W; i++) {
                out += this->get(0, i) * this->cofactor(0, i);
            }
            return out;
        }
    }

    [[nodiscard]] constexpr bool invertible() const {
        return this->determinant() != 0.f;
    }

    [[nodiscard]] constexpr mat<W, H> inverse() const {
        if (!this->invertible()) {
            if constexpr (W == H) {
                return mat<W, H>::make_identity();
            } else {
                return mat<W, H>{};
            }
        }
        mat<W, H> out;
        const float determinant_ = this->determinant();
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                // matrix must be transposed
                // then each element divided by the determinant
                out(j, i) = this->cofactor(i, j) / determinant_;
            }
        }
        return out;
    }

    [[nodiscard]] constexpr int get_width() const {
        return W;
    }
    [[nodiscard]] constexpr int get_height() const {
        return H;
    }

    constexpr void set(float value, int row, int col) {
        this->values[row * W + col] = value;
    }
    [[nodiscard]] constexpr float get(int row, int col) const {
        return this->values[row * W + col];
    }
    [[nodiscard]] constexpr const float& operator()(int row, int col) const {
        return this->values[row * W + col];
    }
    [[nodiscard]] constexpr float& operator()(int row, int col) {
        return this->values[row * W + col];
    }

    [[nodiscard]] constexpr mat<W, H> operator*(mat<W, H> other) const {
        mat<W, H> out;
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                for (int k = 0; k < H; k++) {
                    out(i, j) += this->get(i, k) * other.get(k, j);
                }
            }
        }
        return out;
    }
    constexpr void operator*=(mat<W, H> other) {
        *this = *this * other;
    }
    [[nodiscard]] constexpr vec operator*(vec other) const {
        static_assert(W == 4 && H == 4);
        return {
            this->get(0, 0) * other.x + this->get(0, 1) * other.y + this->get(0, 2) * other.z + this->get(0, 3) * other.w,
            this->get(1, 0) * other.x + this->get(1, 1) * other.y + this->get(1, 2) * other.z + this->get(1, 3) * other.w,
            this->get(2, 0) * other.x + this->get(2, 1) * other.y + this->get(2, 2) * other.z + this->get(2, 3) * other.w,
            this->get(3, 0) * other.x + this->get(3, 1) * other.y + this->get(3, 2) * other.z + this->get(3, 3) * other.w
        };
    }

    [[nodiscard]] constexpr bool operator==(mat<W, H> other) const {
        for (int i = 0; i < W * H; i++) {
            if (!float_eq(this->values[i], other.values[i])) {
                return false;
            }
        }
        return true;
    }

private:
    float values[W * H] {0};
};

} // namespace rt
