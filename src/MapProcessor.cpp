#include <iostream>

#include <Metatile.hpp>
#include <LayoutMetatile.hpp>
#include <MapProcessor.hpp>
#include <global.hpp>

void MapProcessor::processBytes(BytesVector const& bytes, int width) {
    this->m_width = width;

    // First 10 bits are metatile data
    // Then 2 bits for collision data
    // Then 4 bits for elevation data

    this->m_layoutTiles.reserve(bytes->size());

    for (auto it = bytes->begin(); it != bytes->end(); it++) {
        uint16_t const& byte = *it;
        uint16_t metatile = byte & 0x3FF;
        uint8_t collision = (byte >> 10) & 0b11;
        uint8_t elevation = byte >> 12;
        uint8_t advanceMapFormat = (elevation << 2) + collision;

        this->m_layoutTiles.emplace_back(metatile, collision, elevation, advanceMapFormat);
    }
}

void MapProcessor::simpleMode(bool simpleMode) {
    this->m_simple = simpleMode;
}

void MapProcessor::showMetatileInfo(bool show) {
    this->m_showMetatileInfo = show;
}

template <typename T>
void MapProcessor::printField(std::string const& title, T LayoutTile::* field) {
    std::cout << title << '\n';
    std::ios state(nullptr);

    state.copyfmt(std::cout);
    for (int i = 0; i < this->m_layoutTiles.size(); ++i) {
        auto const& tile = this->m_layoutTiles.at(i);
        
        if (i != 0 && i % this->m_width == 0)
            std::cout << '\n';
        
        T const& value = tile.*field;

        if (field == &LayoutTile::collision) {
            if (value != 0)
                std::cout << "\033[1;41m";
            else
                std::cout << "\033[1;42m";
        }
        if (field == &LayoutTile::elevation) {
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

        if (reinterpret_cast<uint8_t LayoutTile::*>(field) == &LayoutTile::advanceMapFormat) {
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

void MapProcessor::printField(std::string const& title, LayoutMetatile LayoutTile::* field) {
    std::cout << title << '\n';
    std::ios state(nullptr);

    state.copyfmt(std::cout);
    for (int i = 0; i < this->m_layoutTiles.size(); ++i) {
        auto const& tile = this->m_layoutTiles.at(i);
        
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

void MapProcessor::drawMetatilePart(std::array<Tile, 4> metatilePart, std::uint16_t layoutIndex, std::vector<std::vector<Pixel>>& output) const {
    int metatileX = (layoutIndex % this->m_width) * 16;
    int metatileY = (layoutIndex / this->m_width) * 16;

    int xOffset = 0;
    int yOffset = 0;

    for (int metatilePartIndex = 0; metatilePartIndex < metatilePart.size(); ++metatilePartIndex) {
        auto const& tile = metatilePart.at(metatilePartIndex);

        std::array<std::array<Pixel, 8>, 8> tilePixels;

        if (tile.isSecTileset()) {
            tilePixels = this->m_secTileset->getTilePixels(tile);
        }
        else {
            tilePixels = this->m_primTileset->getTilePixels(tile);
        }
        
        if (metatilePartIndex >= 2)
            yOffset = 8;

        if (metatilePartIndex % 2 == 1)
            xOffset = 8;
        else
            xOffset = 0;

        // Put the pixels in the bag
        int yTile = 0;
        for (auto const& tileRow : tilePixels) {
            int xTile = 0;
            for (auto const& pixel : tileRow) {
                if (pixel.a != 0)
                    output.at(metatileY + yTile + yOffset).at(metatileX + xTile + xOffset) = pixel;
                ++xTile;
            }
            ++yTile;
        }
    }
}

void MapProcessor::renderActualMap() const {
    auto const& primMetatiles = this->m_primTileset->getMetatiles();
    auto const& secMetatiles = this->m_secTileset->getMetatiles();

    if (this->m_primTileset->getMetatiles().size() == 0) {
        std::cerr << "Call Tileset::readMetatiles() before MapProcessor::renderMetatiles()" << std::endl;
        return;
    }

    int const outputWidth = this->m_width * 16;
    int const outputHeight = ceil(static_cast<float>(this->m_layoutTiles.size()) / this->m_width) * 16;

    // Prefill the vector for easier access
    std::vector<std::vector<Pixel>> output;
    output.reserve(outputHeight);

    std::vector<Pixel> bufferVec;
    bufferVec.reserve(outputWidth);

    for (size_t y = 0; y < outputHeight; ++y) {
        bufferVec.clear();
        for (int x = 0; x < bufferVec.capacity(); x++) {
            bufferVec.emplace_back(0, 0, 0, 0);
        }
        output.push_back(bufferVec);
    }
    
    // Go through the map layout
    for (size_t layoutIndex = 0; layoutIndex < this->m_layoutTiles.size(); ++layoutIndex) {
        auto const& layoutMetatile = this->m_layoutTiles.at(layoutIndex).metatile;

        Metatile metatile;

        if (layoutMetatile.isSecondTileset())
            metatile = secMetatiles.at(layoutMetatile.getTileID());
        else
            metatile = primMetatiles.at(layoutMetatile.getTileID());

        auto const& backgroundTiles = metatile.getBackgroundTiles();
        auto const& foregroundTiles = metatile.getForegroundTiles();
        
        this->drawMetatilePart(backgroundTiles, layoutIndex, output);
        this->drawMetatilePart(foregroundTiles, layoutIndex, output);
    }

    PngHandler outputHandler{global::g_outputPath / "output.png"};
    outputHandler.write(output);
    std::puts("Exported Map");
}

void MapProcessor::renderMap(std::filesystem::path const& outputPath) {
    if (!this->m_primTileset.has_value()) {
        std::cerr << "Can't render the map, please use MapProcessor::setTileset() first!";
        return;
    }
    this->m_primTileset->readMetatiles();
    this->m_secTileset->readMetatiles();

    // this->renderMetatiles();

    this->renderActualMap();
}

std::string MapProcessor::getTilesetFolderName(std::string const& tileset) {
    auto res = tileset.substr(9);

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

    for (int i = 0; i < indexes.size(); ++i) {
        int index = indexes.at(i);
        res.insert(index + i, "_");
    }

    for (char& c : res) {
        c = std::tolower(c);
    }

    return res;
}

void MapProcessor::setTilesets(std::string const& primary, std::string const& secondary) {
    std::filesystem::path const tilesetsPath = global::g_rootPath / "data/tilesets";

    this->m_primTileset = Tileset{tilesetsPath / "primary" / this->getTilesetFolderName(primary)};
    this->m_secTileset = Tileset{tilesetsPath / "secondary" / this->getTilesetFolderName(secondary)};
}

void MapProcessor::printData() {
    if (this->m_simple) {
        this->printField("Collision:", &LayoutTile::collision);
        return;
    }

    if (this->m_showMetatileInfo) {
        this->printField("Metatiles:", &LayoutTile::metatile);
        std::cout << '\n';
        return;
    }

    this->printField("Collision:", &LayoutTile::collision);
    this->printField("\nElevation:", &LayoutTile::elevation);
    this->printField("\nAdvanceMap format:", &LayoutTile::advanceMapFormat);
    std::cout << std::endl;
}

void MapProcessor::renderMetatiles() const {
    auto const& primMetatiles = this->m_primTileset->getMetatiles();
    auto const& secMetatiles = this->m_secTileset->getMetatiles();

    size_t const amountMetatiles = secMetatiles.size();

    if (this->m_primTileset->getMetatiles().size() == 0) {
        std::cerr << "Call Tileset::readMetatiles() before MapProcessor::renderMetatiles()" << std::endl;
        return;
    }

    int const width = 5;

    int const outputHeight = ceil(static_cast<float>(amountMetatiles) / width) * 16;
    int const outputWidth = 16 * width;

    // Prefill the vector for easier access
    std::vector<std::vector<Pixel>> output;
    output.reserve(outputHeight);

    std::vector<Pixel> bufferVec;
    bufferVec.reserve(outputWidth);

    for (size_t y = 0; y < outputHeight; ++y) {
        bufferVec.clear();
        for (int x = 0; x < bufferVec.capacity(); x++) {
            bufferVec.emplace_back(0, 0, 0, 0);
        }
        output.push_back(bufferVec);
    }
    
    // Go through all the metatiles
    for (size_t metatileIndex = 0; metatileIndex < amountMetatiles; ++metatileIndex) {
        auto const& metatile = secMetatiles.at(metatileIndex);
        auto const& backgroundTiles = metatile.getBackgroundTiles();
        auto const& foregroundTiles = metatile.getForegroundTiles();
        
        int metatileX = (metatileIndex % width) * 16;
        int metatileY = (metatileIndex / width) * 16;

        int xOffset = 0;
        int yOffset = 0;

        // Drawing every metatile part (tile)
        for (int metatilePartIndex = 0; metatilePartIndex < backgroundTiles.size(); ++metatilePartIndex) {
            auto const& tile = backgroundTiles.at(metatilePartIndex);

            std::array<std::array<Pixel, 8>, 8> tilePixels;

            if (tile.isSecTileset()) {
                tilePixels = this->m_secTileset->getTilePixels(tile);
            }
            else {
                tilePixels = this->m_primTileset->getTilePixels(tile);
            }
            
            if (metatilePartIndex >= 2)
                yOffset = 8;
        
            if (metatilePartIndex % 2 == 1)
                xOffset = 8;
            else
                xOffset = 0;
    
            // Put the pixels in the bag
            int yTile = 0;
            for (auto const& tileRow : tilePixels) {
                int xTile = 0;
                for (auto const& pixel : tileRow) {
                    output.at(metatileY + yTile + yOffset).at(metatileX + xTile + xOffset) = pixel;
                    ++xTile;
                }
                ++yTile;
            }
        }
    }

    PngHandler outputHandler{global::g_outputPath / "output.png"};
    outputHandler.write(output);
    std::puts("Exported Metatileset");
}