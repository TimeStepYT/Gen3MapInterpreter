#include <iostream>

#include <global.hpp>
#include <Tileset.hpp>
#include <FileHandler.hpp>
#include <fmt/core.h>

Tileset::Tileset(std::filesystem::path const& path) {
    this->m_rootPath = path;
    this->init();
}

Tileset::Tileset(std::filesystem::path&& path) {
    this->m_rootPath = std::move(path);
    this->init();
}

void Tileset::init() {
    this->m_tilesPngPath = this->m_rootPath / "tiles.png";
    this->m_palettePath = this->m_rootPath / "palettes";
}

std::filesystem::path const& Tileset::getPath() const {
    return this->m_rootPath;
}

std::filesystem::path Tileset::getMetatilesBinPath() const {
    return this->m_rootPath / "metatiles.bin";
}

void Tileset::setTilesPngPath(std::filesystem::path const& path) {
    this->m_tilesPngPath = path;
}

std::filesystem::path Tileset::getTilesPngPath() const {
    return this->m_tilesPngPath;
}

void Tileset::setPaletteDir(std::filesystem::path const& path) {
    this->m_palettePath = path;
}

std::filesystem::path Tileset::getPaletteDir() const {
    return this->m_palettePath;
}

std::vector<Metatile> const& Tileset::getMetatiles() const {
    return this->m_metatiles;
}

PngHandler const& Tileset::getTilesPng() {
    if (!this->m_tilesPng) {
        this->m_tilesPng = std::make_unique<PngHandler>(this->getTilesPngPath());
        this->m_tilesPng->read();
    }

    return *this->m_tilesPng;
}

Palette const& Tileset::getPaletteByIndex(int index) {
    auto& cachedPalette = this->m_palettes.at(index);
    if (cachedPalette != nullptr) {
        return *cachedPalette;
    }

    std::ostringstream fileName;

    if (index < 10)
        fileName << 0;

    fileName << index << ".pal";

    auto pal = std::make_unique<Palette>(this->getPaletteDir() / fileName.str());
    cachedPalette = std::move(pal);

    return *cachedPalette;
}

std::array<Pixel, 8 * 8> const& Tileset::getTilePixels(Tile const& tile, Palette& palette) {
    auto const& index = tile.getTileID();

    PngHandler const& tiles = this->getTilesPng();

    static std::array<Pixel, 8 * 8> res;
    
    size_t tilesAmount = (tiles.getWidth() / 8) * (tiles.getHeight() / 8);
    
    // Failsafe
    if (index >= tilesAmount) {
        for (int i = 0; i < 8 * 8; ++i) {
            res[i] = Pixel(0, 0, 0, 0);
        }

        return res;
    }

    int const xTile = (index * 8) % tiles.getWidth();
    int const yTile = ((index * 8) / tiles.getWidth()) * 8;

    for (int i = 0; i < 8 * 8; ++i) {
        int const xRes = i % 8;
        int const yRes = i / 8;

        int pixelX = xTile + xRes;
        int pixelY = yTile + yRes;

        if (tile.isFlippedX())
            pixelX = xTile + (7 - xRes);
        if (tile.isFlippedY())
            pixelY = yTile + (7 - yRes);

        std::uint8_t pixelIndex = tiles.getPixelIndex(pixelX, pixelY);

        if (pixelIndex == 0) { // transparent index
            res[i] = Pixel(0, 0, 0, 0);
            continue;
        }
        
        if (pixelIndex >= palette.getColors().size()) {
            fmt::println("Pixel index {} out of bounds", pixelIndex);
            res[i] = Pixel(0, 0, 0, 0);
            continue;
        }

        auto const& pixel = palette.getColors().at(pixelIndex);
    
        res[i] = pixel;
    }

    return res;
}

bool Tileset::isBroken() const {
    return this->m_broken;
}

void Tileset::readMetatiles() {
    if (this->m_primaryTilesetSize == 0) {
        fmt::println("Primary tileset size is 0!");
    }

    FileHandler metatilesFile;
    bool readingSuccess = metatilesFile.readBinaryFile(this->getMetatilesBinPath());
    
    if (!readingSuccess) {
        this->m_broken = true;
        return;
    }

    auto bytes = metatilesFile.getU16Vector();

    std::array<std::uint16_t, 8> buffer;
    for (int i = 0; i < bytes->size(); ++i) {
        std::uint16_t const& byte = bytes->at(i);
        
        buffer.at(i % 8) = byte;
        
        if (i % 8 == 7) {
            Metatile metatile{buffer, this->m_primaryTilesetSize};
            this->m_metatiles.push_back(metatile);
        }
    }
}

void Tileset::setPrimTilesetSize(size_t size) {
    this->m_primaryTilesetSize = size;
}