#include "bitmap.hpp"

#include <algorithm>
#include <iterator>
#include <utility>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "thirdparty/stb_image_write.h"

using namespace rt;

std::vector<unsigned char> bitmap::get_raw_png() const {
    std::vector<unsigned char> buffer(this->width * this->height * 3);
    for (int i = 0, j = 0; i < this->width * this->height * 3; i += 3, j += 1) {
        buffer[i+0] = std::clamp<unsigned char>(static_cast<unsigned char>(255 * this->pixels[j].r), 0, 255);
        buffer[i+1] = std::clamp<unsigned char>(static_cast<unsigned char>(255 * this->pixels[j].g), 0, 255);
        buffer[i+2] = std::clamp<unsigned char>(static_cast<unsigned char>(255 * this->pixels[j].b), 0, 255);
    }
    int pngBufferSize;
    auto* pngBuffer = stbi_write_png_to_mem(buffer.data(), this->width * 3, this->width, this->height, 3, &pngBufferSize);
    std::vector<unsigned char> out(pngBufferSize);
    std::copy(pngBuffer, pngBuffer + pngBufferSize, std::back_inserter(out));
    STBIW_FREE(pngBuffer);
    return out;
}

bool bitmap::save(std::string_view filepath) const {
    std::vector<std::byte> buffer;
    buffer.resize(this->width * this->height * 3);
    for (int i = 0, j = 0; i < this->width * this->height * 3; i += 3, j += 1) {
        buffer[i+0] = std::clamp(static_cast<std::byte>(255 * this->pixels[j].r), static_cast<std::byte>(0), static_cast<std::byte>(255));
        buffer[i+1] = std::clamp(static_cast<std::byte>(255 * this->pixels[j].g), static_cast<std::byte>(0), static_cast<std::byte>(255));
        buffer[i+2] = std::clamp(static_cast<std::byte>(255 * this->pixels[j].b), static_cast<std::byte>(0), static_cast<std::byte>(255));
    }
    return stbi_write_png(filepath.data(), this->width, this->height, 3, buffer.data(), this->width * 3);
}
