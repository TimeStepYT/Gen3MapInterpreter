#pragma once

#include <vector>
#include <memory>
#include <string>
#include <filesystem>

#include <LayoutMetatile.hpp>
#include <Tileset.hpp>

class MapProcessor {
    using BytesVector = std::unique_ptr<std::vector<uint16_t>>;
    struct LayoutTile {
        LayoutMetatile metatile {0};
        std::uint8_t collision = 0;
        std::uint8_t elevation = 0;
        std::uint8_t advanceMapFormat = 0;
    };
    
    int m_width = 0;
    std::vector<LayoutTile> m_layoutTiles;
    bool m_showMetatileInfo = false;
    bool m_simple = false;
    std::optional<Tileset> m_primTileset;
    std::optional<Tileset> m_secTileset;
    
    template <typename T>
    void printField(std::string const& title, T LayoutTile::* field);
    void printField(std::string const& title, LayoutMetatile LayoutTile::* field);
    // Turns strings like "gTileset_BattlePyramid" into "battle_pyramid"
    std::string getTilesetFolderName(std::string const& tileset);
    void drawMetatilePart(std::array<Tile, 4> metatilePart, std::uint16_t layoutIndex, std::vector<std::vector<Pixel>>& output);
    void renderMetatiles();
    void renderActualMap();
public:
    void renderMap(std::filesystem::path const& outputPath);
    void setTilesets(std::string const& primary, std::string const& secondary);
    void showMetatileInfo(bool show);
    void simpleMode(bool simpleMode);
    void processBytes(BytesVector const& bytes, int width);
    void printData();
};