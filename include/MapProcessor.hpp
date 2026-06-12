#pragma once

#include <vector>
#include <memory>
#include <string>
#include <filesystem>
#include <optional>

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
    std::unique_ptr<Tileset> m_primTileset;
    std::unique_ptr<Tileset> m_secTileset;
    std::string m_mapName = "Map";
    
    template <typename T>
    void printField(std::string const& title, T LayoutTile::* field);
    void printField(std::string const& title, LayoutMetatile LayoutTile::* field);
    // Turns strings like "gTileset_BattlePyramid" into "battle_pyramid"
    std::string getTilesetFolderName(std::string const& tileset);
    void drawMetatilePart(std::array<Tile, 4> metatilePart, std::uint16_t layoutIndex, std::vector<std::vector<Pixel>>& output, bool isSecondTileset);
    void renderMetatiles(std::filesystem::path const& outputPath);
    void renderActualMap(std::filesystem::path const& outputPath);
public:
    void setName(std::string const& name);
    void renderMap(std::filesystem::path const& outputPath);
    void setTilesets(std::string const& primary, std::string const& secondary);
    void showMetatileInfo(bool show);
    void simpleMode(bool simpleMode);
    void processBytes(BytesVector const& bytes, int width);
    void printData();
};