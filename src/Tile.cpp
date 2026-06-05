#include <Tile.hpp>

Tile::Tile(std::uint16_t byte) {
    auto tileID = byte & 0b1111111111;

    if (tileID >= 0x200) {
        tileID -= 0x200;
        this->m_isSecTileset = true;
    }

    this->m_tileID = tileID;
    this->m_paletteID = byte >> (16 - 4);
    this->m_flipX = (byte >> 10) & 0b1;
    this->m_flipY = (byte >> 11) & 0b1;
}

std::uint8_t const& Tile::getPaletteID() const {
    return this->m_paletteID;
}

bool const& Tile::isFlippedX() const {
    return this->m_flipX;
}

bool const& Tile::isFlippedY() const {
    return this->m_flipY;
}

std::uint16_t const& Tile::getTileID() const {
    return this->m_tileID;
}

bool const& Tile::isSecTileset() const {
    return this->m_isSecTileset;
}