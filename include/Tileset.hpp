#pragma once

#include <filesystem>
#include <array>
#include <memory>

#include <Tile.hpp>
#include <Metatile.hpp>
#include <Palette.hpp>
#include <PngHandler.hpp>

class Tileset {
    std::filesystem::path m_rootPath;
    std::vector<Metatile> m_metatiles;
    std::unique_ptr<PngHandler> m_tilesPng = nullptr;
    std::array<std::unique_ptr<Palette>, 16> m_palettes;
public:
    Tileset(std::filesystem::path const&);
    Tileset(std::filesystem::path&&);
    std::filesystem::path const& getPath() const;
    std::filesystem::path getMetatilesBinPath() const;
    std::filesystem::path getTilesPngPath() const;
    std::filesystem::path getPaletteDir() const;
    PngHandler const& getTilesPng();
    Palette const& getPaletteByIndex(int index);
    std::array<std::array<std::unique_ptr<Pixel>, 8>, 8> getTilePixels(Tile const& tile, Palette& palette);
    void readMetatiles();
    std::vector<Metatile> const& getMetatiles() const;
};