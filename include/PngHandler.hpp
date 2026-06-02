#pragma once

#include <filesystem>
#include <png.h>

struct Pixel {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0xff;

    friend std::ostream& operator<<(std::ostream& out, Pixel pixel) {
        return out << std::hex << '#' << 
        static_cast<int>(pixel.r) << 
        static_cast<int>(pixel.g) << 
        static_cast<int>(pixel.b) << 
        // static_cast<int>(pixel.a) <<
        std::dec;
    }
};

class PngHandler {
    std::filesystem::path m_path;
    png_struct* m_png = nullptr;
    png_infop m_info = nullptr;
    std::vector<std::vector<Pixel>> m_rows;
    
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    int m_bitDepth = 0;
    int m_colorType = 0;

    void readDetails();
    void readPixels();
    void deepCopyRows(png_byte** rows, size_t rowSize);
public:
    PngHandler(std::filesystem::path const& path);

    void read();
    void write();
};