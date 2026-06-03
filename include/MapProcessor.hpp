#pragma once

#include <vector>
#include <memory>
#include <string>
#include <filesystem>

#include <LayoutMetatile.hpp>

class MapProcessor {
    using BytesVector = std::unique_ptr<std::vector<uint16_t>>;
    struct Tile {
        LayoutMetatile metatile {0};
        uint8_t collision = 0;
        uint8_t elevation = 0;
        uint8_t advanceMapFormat = 0;
    };
    
    int m_width = 0;
    std::vector<Tile> m_tiles;
    bool m_showMetatileInfo = false;
    bool m_simple = false;
    std::filesystem::path m_primTilesetPath;
    std::filesystem::path m_secTilesetPath;
    
    template <typename T>
    void printField(std::string const& title, T Tile::* field);
    void printField(std::string const& title, LayoutMetatile Tile::* field);
    std::string getTilesetFolderName(std::string const& tileset);
public:

    void setTilesets(std::string const& primary, std::string const& secondary);
    void showMetatileInfo(bool show);
    void simpleMode(bool simpleMode);
    void processBytes(BytesVector const& bytes, int width);
    void printData();
};