#include <LayoutMetatile.hpp>

LayoutMetatile::LayoutMetatile(uint16_t metatileID) {
    if (metatileID >= 0x200) {
        this->m_secondTileset = true;
        this->m_tileID = metatileID - 0x200;
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