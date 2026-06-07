#pragma once

#include <ostream>

class Pixel {
    public:
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 0xff;

    friend std::ostream& operator<<(std::ostream& out, Pixel pixel);
};