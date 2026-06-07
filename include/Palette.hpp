#pragma once

#include <Pixel.hpp>
#include <vector>
#include <filesystem>

class Palette {
    std::vector<Pixel> m_colors;

public:
    static Palette fromFile(std::filesystem::path filePath);
    
    void setColors(std::vector<Pixel> const& colors);
    void setColor(int index, Pixel const& color);

    std::vector<Pixel> const& getColors();
};