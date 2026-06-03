#pragma once

#include <stdint.h>

class LayoutMetatile {
    bool m_secondTileset = false;
    uint16_t m_tileID = 0;

    public:
    LayoutMetatile(uint16_t metatileIndex);

    bool isSecondTileset() const;
    uint16_t getTileID() const;
};