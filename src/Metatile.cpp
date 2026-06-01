#include <Metatile.hpp>

Metatile::Metatile(uint16_t metatileID) {
    if (metatileID >= 0x200) {
        this->m_secondTileset = true;
        this->m_tileID = metatileID - 0x200;
    }
    else {
        this->m_tileID = metatileID;
    }
}

bool Metatile::isSecondTileset() const {
    return this->m_secondTileset;
}

uint16_t Metatile::getTileID() const {
    return this->m_tileID;
}