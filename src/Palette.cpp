#include <fstream>
#include <iostream>

#include <array>

#include <Palette.hpp>

Palette::Palette(std::filesystem::path filePath) {
    std::ifstream file(filePath);

    std::string line;

    int lineNumber = -1;
    int paletteSize = 0;
    int colorIndex = 0;

    std::array<int, 3> valueBuffer;

    while((paletteSize == 0 || colorIndex < paletteSize) && std::getline(file, line)) {
        ++lineNumber;

        if (lineNumber < 2)
            continue;

        if (lineNumber == 2) {
            paletteSize = std::stoi(line);
            this->m_colors.reserve(paletteSize);
            continue;
        }
        
        std::istringstream values(line);

        std::string valueString;
        
        for (int i = 0; i < 3; ++i) {
            std::getline(values, valueString, ' ');
            int value = std::stoi(valueString);
            valueBuffer[i] = value;
        }

        this->m_colors.emplace_back(valueBuffer[0], valueBuffer[1], valueBuffer[2]);
        ++colorIndex;
    }
}

void Palette::setColors(std::vector<Pixel> const& colors) {
    this->m_colors = colors;
}

void Palette::setColor(int index, Pixel const& color) {
    if (index >= this->m_colors.size())
        this->m_colors.resize(index + 1);

    this->m_colors.at(index) = color;
}

std::vector<Pixel> const& Palette::getColors() {
    return this->m_colors;
}