#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "color.hpp"

namespace rt {

struct bitmap {
    bitmap(short width_, short height_)
            : width(width_)
            , height(height_)
            , pixels(std::make_unique<color[]>(width_ * height_)) {}
    bitmap(const bitmap& other) {
        this->width = other.width;
        this->height = other.height;
        this->pixels = std::make_unique<color[]>(other.width * other.height);
        std::memcpy(this->pixels.get(), other.pixels.get(), sizeof(color) * this->width * this->height);
    }
    bitmap& operator=(const bitmap& other) {
        this->width = other.width;
        this->height = other.height;
        this->pixels = std::make_unique<color[]>(other.width * other.height);
        std::memcpy(this->pixels.get(), other.pixels.get(), sizeof(color) * this->width * this->height);
        return *this;
    }

    [[nodiscard]] short get_width() const {
        return this->width;
    }
    [[nodiscard]] short get_height() const {
        return this->height;
    }

    void set_pixel(color c, short width_, short height_) {
        this->pixels[this->width * height_ + width_] = c;
    }
    [[nodiscard]] color get_pixel(short width_, short height_) const {
        return this->pixels[this->width * height_ + width_];
    }
    [[nodiscard]] color& operator()(short width_, short height_) {
        return this->pixels[this->width * height_ + width_];
    }

    [[nodiscard]] std::vector<unsigned char> get_raw_png() const;
    bool save(std::string_view filepath) const; // NOLINT(modernize-use-nodiscard)

private:
    short width;
    short height;
    std::unique_ptr<color[]> pixels;
};

} // namespace rt
