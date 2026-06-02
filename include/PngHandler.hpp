#pragma once

#include <filesystem>
#include <png.h>

struct Pixel {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0xff;

    friend std::ostream& operator<<(std::ostream& out, Pixel pixel) {
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
    void writePixels();
    void deepCopyRows(png_byte** rows, size_t rowSize);
public:
    PngHandler(std::filesystem::path const& path);

    std::vector<std::vector<Pixel>> const& getAllPixels() const;
    Pixel const& getPixel(unsigned int x, unsigned int y) const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    void read();
    void write(std::vector<std::vector<Pixel>> const& pixelVector);
};