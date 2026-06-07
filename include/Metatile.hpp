#pragma once

#include <array>
#include <vector>
#include <cstdint>

#include <Tile.hpp>

class Metatile {
    std::array<std::uint16_t, 8> m_data;
    std::array<Tile, 4> m_foregroundTiles;
    std::array<Tile, 4> m_backgroundTiles;

public:
    Metatile() = default;
    Metatile(std::array<std::uint16_t, 8> const& array);
    std::array<Tile, 4> const& getBackgroundTiles() const;
    std::array<Tile, 4> const& getForegroundTiles() const;
};