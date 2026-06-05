#include <Metatile.hpp>

Metatile::Metatile(std::array<std::uint16_t, 8> const& array) {
    for (int i = 0; i < 8; ++i) {
        std::uint16_t const& byte = array.at(i);

        if (i < 4)
            this->m_backgroundTiles.at(i) = byte;
        else
            this->m_foregroundTiles.at(i - 4) = byte;
    }
}

std::array<Tile, 4> const& Metatile::getBackgroundTiles() const {
    return this->m_backgroundTiles;
};

std::array<Tile, 4> const& Metatile::getForegroundTiles() const {
    return this->m_foregroundTiles;
};