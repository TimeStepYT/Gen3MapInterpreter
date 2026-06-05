#pragma once

#include <cstdint>

#include <Palette.hpp>

class Tile {
    std::uint16_t m_tileID = 0;
    std::uint8_t m_paletteID = 0;
    bool m_flipX = false;
    bool m_flipY = false;
    bool m_isSecTileset = false;
public:
    Tile() = default;
    Tile(std::uint16_t byte);
    std::uint8_t const& getPaletteID() const;
    std::uint16_t const& getTileID() const;
    bool const& isFlippedX() const;
    bool const& isFlippedY() const;
    bool const& isSecTileset() const;
};