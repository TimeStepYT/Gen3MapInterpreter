#pragma once

#include <vector>
#include <memory>
#include <string>

#include <Metatile.hpp>

class MapProcessor {
    using BytesVector = std::unique_ptr<std::vector<uint16_t>>;
    struct Tile {
        Metatile metatile {0};
        uint8_t collision = 0;
        uint8_t elevation = 0;
        uint8_t advanceMapFormat = 0;
    };
    
    int m_width = 0;
    std::vector<Tile> m_tiles;
    bool m_showMetatileInfo = false;
    bool m_simple = false;
    
    template <typename T>
    void printField(std::string const& title, T Tile::* field);
    void printField(std::string const& title, Metatile Tile::* field);
public:

    void showMetatileInfo(bool show);
    void simpleMode(bool simpleMode);
    void processBytes(BytesVector const& bytes, int width);
    void printData();
};