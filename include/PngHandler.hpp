#pragma once

#include <filesystem>
#include <optional>
#include <png.h>
#include <functional>

#include <Palette.hpp>
#include <Pixel.hpp>

class PngHandler {
    std::filesystem::path m_path;
    png_struct* m_png = nullptr;
    png_infop m_info = nullptr;
    std::vector<Pixel> m_pixels;
    std::vector<std::uint8_t> m_indexes;
    
    std::uint32_t m_width = 0;
    std::uint32_t m_height = 0;
    int m_bitDepth = 0;
    int m_colorType = 0;

    bool m_hasRead = false;

    void readDetails();
    void readPixels();
    void deepCopyRows(png_byte** rows, size_t rowSize);
    void writePixels();
    void writePipeline();
public:
    PngHandler(std::filesystem::path const& path);
    PngHandler(std::filesystem::path&& path);

    std::vector<Pixel> const& getAllPixels() const;
    std::uint8_t const& getPixelIndex(unsigned int x, unsigned int y) const;
    std::uint32_t getWidth() const;
    std::uint32_t getHeight() const;

    static void writeAsync(std::filesystem::path const& path, std::string const& fileName, size_t width, size_t height, std::vector<Pixel> const& pixels, std::function<void()> callback);

    void read();
    void write(std::vector<Pixel> const& pixelVector, size_t width, size_t height);
    void write(std::vector<Pixel>&& pixelVector, size_t width, size_t height);
};