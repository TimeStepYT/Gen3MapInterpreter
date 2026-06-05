#pragma once

#include <filesystem>
#include <array>

#include <Tile.hpp>
#include <Metatile.hpp>
#include <Palette.hpp>
#include <PngHandler.hpp>

class Tileset {
    std::filesystem::path m_rootPath;
    std::vector<Metatile> m_metatiles;
public:
    Tileset(std::filesystem::path const&);
    Tileset(std::filesystem::path&&);
    std::filesystem::path const& getPath() const;
    std::filesystem::path getMetatilesBinPath() const;
    std::filesystem::path getTilesPngPath() const;
    std::filesystem::path getPaletteDir() const;
    PngHandler getTilesPngWithPalette(Palette const& palette) const;
    Palette getPaletteByIndex(int index) const;
    std::array<std::array<Pixel, 8>, 8> getTilePixels(Tile const& tile) const;
    void readMetatiles();
    std::vector<Metatile> const& getMetatiles() const;
};