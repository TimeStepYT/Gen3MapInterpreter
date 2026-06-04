#include <iostream>

#include <LayoutMetatile.hpp>
#include <MapProcessor.hpp>
#include <global.hpp>

void MapProcessor::processBytes(BytesVector const& bytes, int width) {
    this->m_width = width;

    // First 10 bits are metatile data
    // Then 2 bits for collision data
    // Then 4 bits for elevation data

    this->m_tiles.reserve(bytes->size());

    for (auto it = bytes->begin(); it != bytes->end(); it++) {
        uint16_t const& byte = *it;
        uint16_t metatile = byte & 0x3FF;
        uint8_t collision = (byte >> 10) & 0b11;
        uint8_t elevation = byte >> 12;
        uint8_t advanceMapFormat = (elevation << 2) + collision;

        this->m_tiles.emplace_back(metatile, collision, elevation, advanceMapFormat);
    }
}

void MapProcessor::simpleMode(bool simpleMode) {
    this->m_simple = simpleMode;
}

void MapProcessor::showMetatileInfo(bool show) {
    this->m_showMetatileInfo = show;
}

template <typename T>
void MapProcessor::printField(std::string const& title, T Tile::* field) {
    std::cout << title << '\n';
    std::ios state(nullptr);

    state.copyfmt(std::cout);
    for (int i = 0; i < this->m_tiles.size(); ++i) {
        auto const& tile = this->m_tiles.at(i);
        
        if (i != 0 && i % this->m_width == 0)
            std::cout << '\n';
        
        T const& value = tile.*field;

        if (field == &Tile::collision) {
            if (value != 0)
                std::cout << "\033[1;41m";
            else
                std::cout << "\033[1;42m";
        }
        if (field == &Tile::elevation) {
            switch (value) {
            case 1:
                std::cout << "\033[1;44m";
                break;
            default:
                auto colorValue = value * 51;
                std::cout << std::dec << "\033[48;2;" << colorValue << ";" << colorValue << ";" << colorValue << "m";
                break;
            }
        }
        
        std::cout << std::hex << static_cast<int>(value);

        if (reinterpret_cast<uint8_t Tile::*>(field) == &Tile::advanceMapFormat) {
            if (value <= 0xf)
                std::cout << ' ';
        }
        else {
            std::cout << ' ';
        }

        std::cout << "\033[0m";
    }
    std::cout.copyfmt(state);
    std::cout << '\n';
}

void MapProcessor::printField(std::string const& title, LayoutMetatile Tile::* field) {
    std::cout << title << '\n';
    std::ios state(nullptr);

    state.copyfmt(std::cout);
    for (int i = 0; i < this->m_tiles.size(); ++i) {
        auto const& tile = this->m_tiles.at(i);
        
        if (i != 0 && i % this->m_width == 0)
            std::cout << '\n';
        
        LayoutMetatile const& value = tile.*field;

        std::cout << "(" << static_cast<int>(value.isSecondTileset()) + 1 << ") " << value.getTileID() << ' ';

        if (value.getTileID() < 10)
            std::cout << "  ";
        else if (value.getTileID() < 100)
            std::cout << ' ';

        std::cout << "\033[0m";
    }
    std::cout.copyfmt(state);
    std::cout << '\n';
}

std::string MapProcessor::getTilesetFolderName(std::string const& tileset) {
    // I need to turn "gTileset_BattlePyramid" into "battle_pyramid".
    // If I notice any inconsistencies where this is not enough to turn it into the folder name,
    // then I'll just add it to an if else block whatever

    // Get rid of the prefix "gTileset_"
    auto res = tileset.substr(9);

    // Get indexes for where to add underscores
    std::vector<int> indexes;

    bool prevCapitalized = true;
    for (int i = 0; i < res.size(); ++i) {
        char c = res.at(i);

        bool isCapitalized = std::isupper(c);

        if (!prevCapitalized && isCapitalized) {
            indexes.push_back(i);
        }
        
        prevCapitalized = isCapitalized;
    }

    // Add underscores
    for (int i = 0; i < indexes.size(); ++i) {
        int index = indexes.at(i);
        res.insert(index + i, "_");
    }

    // Turn everything lower case
    for (char& c : res) {
        c = std::tolower(c);
    }

    return res;
}

void MapProcessor::setTilesets(std::string const& primary, std::string const& secondary) {
    std::filesystem::path const tilesetsPath = global::g_rootPath / "data/tilesets";

    this->m_primTilesetPath = tilesetsPath / "primary" / this->getTilesetFolderName(primary);
    this->m_secTilesetPath = tilesetsPath / "secondary" / this->getTilesetFolderName(secondary);
}

void MapProcessor::printData() {
    if (this->m_simple) {
        this->printField("Collision:", &Tile::collision);
        return;
    }

    if (this->m_showMetatileInfo) {
        this->printField("Metatiles:", &Tile::metatile);
        std::cout << '\n';
        return;
    }

    this->printField("Collision:", &Tile::collision);
    this->printField("\nElevation:", &Tile::elevation);
    this->printField("\nAdvanceMap format:", &Tile::advanceMapFormat);
    std::cout << std::endl;
}