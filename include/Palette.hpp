#pragma once

#include <png.h>
#include <vector>
#include <filesystem>

class Palette {
    std::vector<png_color> m_colors;

public:
    static Palette fromFile(std::filesystem::path filePath);
    
    void setColors(std::vector<png_color> const& colors);
    void setColor(int index, png_color const& color);

    std::vector<png_color> const& getColors();
};