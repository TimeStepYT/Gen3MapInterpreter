#include <MapMetatile.hpp>

MapMetatile::MapMetatile(uint16_t metatileID) {
    if (metatileID >= 0x200) {
        this->m_secondTileset = true;
        this->m_tileID = metatileID - 0x200;
    }
    else {
        this->m_tileID = metatileID;
    }
}

bool MapMetatile::isSecondTileset() const {
    return this->m_secondTileset;
}

uint16_t MapMetatile::getTileID() const {
    return this->m_tileID;
}