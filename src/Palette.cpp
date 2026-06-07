#include <fstream>
#include <iostream>

#include <array>

#include <Palette.hpp>

Palette Palette::fromFile(std::filesystem::path filePath) {
    Palette res;

    std::ifstream file(filePath);

    std::string line;

    int lineNumber = -1;

    std::array<int, 3> valueBuffer;

    while(std::getline(file, line)) {
        ++lineNumber;

        if (lineNumber < 2)
            continue;

        if (lineNumber == 2) {
            res.m_colors.reserve(std::stoi(line));
            continue;
        }

        // if (lineNumber == 3) {
        //     res.m_colors.emplace_back(0xde, 0xad, 0xbe, 0xef);
        //     continue;
        // }
        
        std::istringstream values(line);

        std::string valueString;
        
        int i = 0;
        while (std::getline(values, valueString, ' ')) {
            int value = std::stoi(valueString);
            valueBuffer[i] = value;
            ++i;
        }

        res.m_colors.emplace_back(valueBuffer[0], valueBuffer[1], valueBuffer[2]);
    }

    return res;
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