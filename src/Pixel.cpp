#include <Pixel.hpp>

std::ostream& operator<<(std::ostream& out, Pixel pixel) {
    out << std::hex << '#';
    
    if (pixel.r < 0x10)
        out << 0;
    out << static_cast<int>(pixel.r);

    if (pixel.g < 0x10)
        out << 0;
    out << static_cast<int>(pixel.g);

    if (pixel.b < 0x10)
        out << 0;
    out << static_cast<int>(pixel.b);

    // if (pixel.a < 0x10)
    //     out << 0;
    // out << static_cast<int>(pixel.a);

    out << std::dec;

    return out;
}