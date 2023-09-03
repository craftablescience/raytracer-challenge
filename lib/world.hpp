#pragma once

#include <optional>
#include <string_view>
#include <vector>
#include "bitmap.hpp"
#include "ray.hpp"

namespace rt {

class transform {
    // todo: rotation, shear
public:
    constexpr transform() {
        this->recalculateMatrix();
    }
    explicit constexpr transform(vec translation, vec scale = vec::make_vector(1, 1, 1))
            : translationVec(translation)
            , scaleVec(scale) {
        this->recalculateMatrix();
    }

    [[nodiscard]] constexpr const mat<4, 4>& get_transform() const {
        return this->matrix;
    }
    [[nodiscard]] constexpr mat<4, 4>& get_transform() {
        return this->matrix;
    }

    constexpr void translate(vec translation) {
        this->translationVec += translation;
        this->recalculateMatrix();
    }
    constexpr void set_translation(vec translation) {
        this->translationVec = translation;
        this->recalculateMatrix();
    }
    [[nodiscard]] constexpr vec get_translation() const {
        return this->translationVec;
    }

    constexpr void add_scale(vec scale) {
        this->scaleVec += scale;
        this->recalculateMatrix();
    }
    constexpr void set_scale(float scale) {
        this->scaleVec = vec::make_vector(scale, scale, scale);
        this->recalculateMatrix();
    }
    constexpr void set_scale(vec scale) {
        this->scaleVec = scale;
        this->recalculateMatrix();
    }
    [[nodiscard]] constexpr vec get_scale() const {
        return this->scaleVec;
    }

private:
    constexpr void recalculateMatrix() {
        this->matrix = mat<4, 4>::make_translation(this->translationVec) * mat<4, 4>::make_scaled(this->scaleVec);
    }

    mat<4,4> matrix;
    vec translationVec = vec::make_point(0, 0, 0);
    vec scaleVec = vec::make_vector(1, 1, 1);
};

struct intersection {
    ray rayHit;
    float distance;
    std::size_t objectID;

    [[nodiscard]] constexpr bool operator==(const intersection& other) const = default;

    [[nodiscard]] static std::optional<intersection> discard_occluded(const std::vector<intersection>& intersections) {
        if (intersections.empty())
            return {};
        intersection best = intersections[0];
        for (const auto intersection : intersections) {
            if ((best.distance < 0 && intersection.distance >= 0) || (intersection.distance <= best.distance)) {
                best = intersection;
            }
        }
        if (best.distance >= 0) {
            return best;
        }
        return {};
    }
};

enum class object_type {
    SPHERE,
};

struct object {
    object_type type;
    std::size_t id;
    transform model{};
    constexpr object(object_type type_, std::size_t id_) : type(type_), id(id_) {}
    virtual ~object() = default;

    [[nodiscard]] constexpr bool operator==(const object& other) const {
        return this->id == other.id;
    }

    [[nodiscard]] virtual constexpr bool intersects(ray r) const = 0;
    [[nodiscard]] virtual std::vector<intersection> intersections(ray r) const = 0;
};

struct sphere : public object {
    explicit constexpr sphere(std::size_t id_) : object(object_type::SPHERE, id_) {}

    [[nodiscard]] constexpr bool intersects(ray r) const override {
        r *= this->model.get_transform().inverse();
        auto sphereToRay = r.origin - vec::make_point(0,0,0);
        auto a = r.direction * r.direction;
        auto b = r.direction * sphereToRay * 2;
        auto c = sphereToRay * sphereToRay - 1;
        auto d = b * b - (4 * a * c);
        return d >= 0;
    }
    [[nodiscard]] std::vector<intersection> intersections(ray r) const override {
        r *= this->model.get_transform().inverse();
        auto sphereToRay = r.origin - vec::make_point(0,0,0);
        auto a = r.direction * r.direction;
        auto b = r.direction * sphereToRay * 2;
        auto c = sphereToRay * sphereToRay - 1;
        auto d = b * b - (4 * a * c);
        std::vector<intersection> out;
        if (d >= 0) {
            out.push_back({r, (-b - std::sqrtf(d)) / (2 * a), this->id});
            out.push_back({r, (-b + std::sqrtf(d)) / (2 * a), this->id});
        }
        return out;
    }
};

class world {
public:
    world() = default;
    ~world() {
        for (const auto object : objects) {
            delete object;
        }
    }

    template<typename T>
    requires std::is_base_of_v<object, T>
    std::size_t add(vec origin, vec scale) {
        auto* s = new T{this->objects.size()};
        s->model.set_translation(origin);
        s->model.set_scale(scale);
        this->objects.push_back(s);
        return this->objects.size() - 1;
    }

    [[nodiscard]] std::vector<intersection> get_intersections(ray r) const {
        std::vector<intersection> out;
        for (const auto object : this->objects) {
            if (auto intersections = object->intersections(r); !intersections.empty()) {
                out.insert(out.end(), intersections.begin(), intersections.end());
            }
        }
        return out;
    }
    [[nodiscard]] std::optional<intersection> get_visible_intersection(ray r) const {
        return intersection::discard_occluded(this->get_intersections(r));
    }

    [[nodiscard]] object* get_object(std::size_t index) const {
        return this->objects.at(index);
    }
    [[nodiscard]] const std::vector<object*>& get_objects() const {
        return this->objects;
    }

    [[nodiscard]] bitmap render(short width, short height, vec camOrigin, vec camDirectionFwd, vec camDirectionUp, float camFov) const {
        bitmap pixels{width, height};

        // https://stackoverflow.com/questions/5944109/raytracer-computing-eye-rays
        for (short y = 0; y < height; y++) {
            for (short x = 0; x < width; x++) {
                vec pointOnScreen = camDirectionFwd * ((static_cast<float>(height) / 2) / std::tanf(camFov / 2)) +
                                    -camDirectionUp * (y - (height / 2)) +
                                    camDirectionFwd.cross(camDirectionUp) * (x - (width / 2));
                ray r{camOrigin, pointOnScreen.normalize()};
                if (this->get_visible_intersection(r)) {
                    pixels.set_pixel({1,0,0}, x, y);
                }
            }
        }
        return pixels;
    }

private:
    std::vector<object*> objects;
};

} // namespace rt
