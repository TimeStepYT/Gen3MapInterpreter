#pragma once

#include <vector>
#include <memory>

class MapProcessor {
    using BytesVector = std::unique_ptr<std::vector<uint16_t>>;
    struct Tile {
        uint16_t metatile = 0;
        uint8_t collision = 0;
        uint8_t elevation = 0;
        uint8_t advanceMapFormat = 0;
    };

    int m_width = 0;
    std::vector<Tile> m_tiles;

    template <typename T>
    void printField(T Tile::* field);
public:
    static inline bool m_simple = false;

    void processBytes(BytesVector const& bytes, int width);
    void printData();
};