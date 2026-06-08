#pragma once

#include <cstdint>

class LayoutMetatile {
    bool m_secondTileset = false;
    std::uint16_t m_tileID = 0;

    public:
    LayoutMetatile(std::uint16_t metatileIndex);

    bool isSecondTileset() const;
    std::uint16_t getTileID() const;
};