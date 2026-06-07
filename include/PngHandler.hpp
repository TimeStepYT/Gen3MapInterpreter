#pragma once

#include <filesystem>
#include <optional>
#include <png.h>

#include <Palette.hpp>
#include <Pixel.hpp>

class PngHandler {
    std::filesystem::path m_path;
    png_struct* m_png = nullptr;
    png_infop m_info = nullptr;
    std::vector<std::vector<Pixel>> m_rows;
    
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    int m_bitDepth = 0;
    int m_colorType = 0;

    std::optional<Palette> m_newPalette;
    bool m_hasRead = false;

    void readDetails();
    void readPixels();
    void deepCopyRows(png_byte** rows, size_t rowSize);
    void writePixels();
    void writePipeline();
public:
    PngHandler(std::filesystem::path const& path);

    std::vector<std::vector<Pixel>> const& getAllPixels() const;
    Pixel const& getPixel(unsigned int x, unsigned int y) const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;

    void setPalette(Palette const& palette);

    void read();
    void write(std::vector<std::vector<Pixel>> const& pixelVector);
    void write(std::vector<std::vector<Pixel>>&& pixelVector);
};