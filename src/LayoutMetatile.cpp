#include <LayoutMetatile.hpp>

LayoutMetatile::LayoutMetatile(uint16_t metatileID, size_t primaryMetatilesetSize) {
    if (metatileID >= primaryMetatilesetSize) {
        this->m_secondTileset = true;
        this->m_tileID = metatileID - primaryMetatilesetSize;
    }
    else {
        this->m_tileID = metatileID;
    }
}

bool LayoutMetatile::isSecondTileset() const {
    return this->m_secondTileset;
}

uint16_t LayoutMetatile::getTileID() const {
    return this->m_tileID;
}