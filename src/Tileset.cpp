#include <iostream>

#include <global.hpp>
#include <Tileset.hpp>
#include <FileHandler.hpp>

Tileset::Tileset(std::filesystem::path const& path) {
    this->m_rootPath = path;
    this->m_tilesPng = this->getTilesPngPath();
    this->m_tilesPng.read();
}

Tileset::Tileset(std::filesystem::path&& path) {
    this->m_rootPath = std::move(path);
    this->m_tilesPng = this->getTilesPngPath();
    this->m_tilesPng.read();
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

PngHandler const& Tileset::getTilesPng() const {
    return this->m_tilesPng;
}

Palette Tileset::getPaletteByIndex(int index) const {
    std::ostringstream fileName;

    if (index < 10)
        fileName << 0;

    fileName << index << ".pal";

    Palette pal = Palette::fromFile(this->getPaletteDir() / fileName.str());

    return pal;
}

std::array<std::array<Pixel, 8>, 8> Tileset::getTilePixels(Tile const& tile) const {
    auto const& index = tile.getTileID();

    Palette palette = this->getPaletteByIndex(tile.getPaletteID());
    PngHandler const& tiles = this->getTilesPng();

    std::array<std::array<Pixel, 8>, 8> res;
    
    size_t tilesAmount = (tiles.getWidth() / 8) * (tiles.getHeight() / 8);
    
    if (index >= tilesAmount) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                res.at(i).at(j) = Pixel{0, 0, 0, 0};
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
                res.at(yRes).at(xRes) = Pixel{0, 0, 0, 0};
                continue;
            }
            
            if (pixelIndex >= palette.getColors().size()) {
                std::cout << "Pixel index " << pixelIndex << " out of bounds" << std::endl;
                res.at(yRes).at(xRes) = Pixel{0, 0, 0, 0};
                continue;
            }

            auto pixel = palette.getColors().at(pixelIndex);
        
            res.at(yRes).at(xRes) = pixel;
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