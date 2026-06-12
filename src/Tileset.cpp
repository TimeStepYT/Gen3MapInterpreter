#include <iostream>

#include <global.hpp>
#include <Tileset.hpp>
#include <FileHandler.hpp>

Tileset::Tileset(std::filesystem::path const& path) {
    this->m_rootPath = path;
}

Tileset::Tileset(std::filesystem::path&& path) {
    this->m_rootPath = std::move(path);
}

std::filesystem::path const& Tileset::getPath() const {
    return this->m_rootPath;
}

std::filesystem::path Tileset::getMetatilesBinPath() const {
    return this->m_rootPath / "metatiles.bin";
}

std::filesystem::path Tileset::getTilesPngPath() const {
    return this->m_rootPath / "tiles.png";
}

std::filesystem::path Tileset::getPaletteDir() const {
    return this->m_rootPath / "palettes";
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

std::array<std::array<std::unique_ptr<Pixel>, 8>, 8> Tileset::getTilePixels(Tile const& tile, Palette& palette) {
    auto const& index = tile.getTileID();

    PngHandler const& tiles = this->getTilesPng();

    std::array<std::array<std::unique_ptr<Pixel>, 8>, 8> res;
    
    size_t tilesAmount = (tiles.getWidth() / 8) * (tiles.getHeight() / 8);
    
    if (index >= tilesAmount) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                res.at(i).at(j) = std::make_unique<Pixel>(0, 0, 0, 0);
            }
        }

        return res;
    }

    int xTile = (index * 8) % tiles.getWidth();
    int yTile = ((index * 8) / tiles.getWidth()) * 8;

    for (int yRes = 0; yRes < 8; ++yRes) {
        for (int xRes = 0; xRes < 8; ++xRes) {
            int pixelX = xTile + xRes;
            int pixelY = yTile + yRes;

            if (tile.isFlippedX())
                pixelX = xTile + (7 - xRes);
            if (tile.isFlippedY())
                pixelY = yTile + (7 - yRes);

            std::uint8_t pixelIndex = tiles.getPixelIndex(pixelX, pixelY);

            if (pixelIndex == 0) { // transparent index
                res.at(yRes).at(xRes) = std::make_unique<Pixel>(0, 0, 0, 0);
                continue;
            }
            
            if (pixelIndex >= palette.getColors().size()) {
                std::cout << "Pixel index " << pixelIndex << " out of bounds" << std::endl;
                res.at(yRes).at(xRes) = std::make_unique<Pixel>(0, 0, 0, 0);
                continue;
            }

            auto const& pixel = palette.getColors().at(pixelIndex);
        
            res.at(yRes).at(xRes) = std::make_unique<Pixel>(pixel);
        }
    }

    return res;
}

void Tileset::readMetatiles() {
    FileHandler metatilesFile;
    metatilesFile.readBinaryFile(this->getMetatilesBinPath());
    auto bytes = metatilesFile.getU16Vector();

    std::array<std::uint16_t, 8> buffer;
    for (int i = 0; i < bytes->size(); ++i) {
        std::uint16_t const& byte = bytes->at(i);
        
        buffer.at(i % 8) = byte;
        
        if (i % 8 == 7) {
            Metatile metatile{buffer};
            this->m_metatiles.push_back(metatile);
        }
    }
}