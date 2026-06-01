#include <stdint.h>

#pragma once

class MapMetatile {
    bool m_secondTileset = false;
    uint16_t m_tileID = 0;

    public:
    MapMetatile(uint16_t metatileIndex);

    bool isSecondTileset() const;
    uint16_t getTileID() const;
};