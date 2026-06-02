#include <PngHandler.hpp>

#include <iostream>
#include <stdio.h>

PngHandler::PngHandler(std::filesystem::path const& path) {
    this->m_path = path;
}

uint32_t PngHandler::getWidth() const {
    return this->m_width;
}

uint32_t PngHandler::getHeight() const {
    return this->m_height;
}

Pixel const& PngHandler::getPixel(unsigned int x, unsigned int y) const {
    return this->m_rows[y][x];
}

std::vector<std::vector<Pixel>> const& PngHandler::getAllPixels() const {
    return this->m_rows;
}

void PngHandler::readDetails() {
    png_get_IHDR(
        this->m_png,
        this->m_info,
        &this->m_width,
        &this->m_height,
        &this->m_bitDepth,
        &this->m_colorType,
        nullptr,
        nullptr,
        nullptr
    );
}

void PngHandler::readPixels() {
    auto* const png = this->m_png;
    auto* const info = this->m_info;
    auto const height = this->m_height;

    bool changedInfo = false;

    if (this->m_colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
        changedInfo = true;
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
        changedInfo = true;
    }

    if (changedInfo) {
        png_read_update_info(png, info);

        this->m_bitDepth = png_get_bit_depth(png, info);
    }

    size_t rowSize = png_get_rowbytes(png, info);

    auto rows = new png_byte*[height];
    for (int i = 0; i < height; ++i) {
        rows[i] = new png_byte[rowSize];
    }

    png_read_image(png, rows);
    png_read_end(png, info);
    
    this->deepCopyRows(rows, rowSize);

    for (int i = 0; i < height; ++i) {
        delete[] rows[i];
    }
    delete[] rows;
}

void PngHandler::deepCopyRows(png_byte** rows, size_t rowSize) {
    uint8_t bytesPerPixel = rowSize / this->m_width;

    this->m_rows.reserve(this->m_height);

    for (int y = 0; y < this->m_height; ++y) {
        std::vector<Pixel> newVec;
        newVec.reserve(this->m_width);

        for (int x = 0; x < this->m_width; ++x) {
            Pixel newPixel{
                rows[y][x*bytesPerPixel],
                rows[y][x*bytesPerPixel + 1],
                rows[y][x*bytesPerPixel + 2]
            };
            
            if (bytesPerPixel == 4)
                newPixel.a = rows[y][x*bytesPerPixel + 3];
            newVec.emplace_back(newPixel);
        }
        this->m_rows.emplace_back(newVec);
    }
}

void PngHandler::read() {
    auto const& path = this->m_path;

    FILE* file;

    errno_t err = fopen_s(&file, path.string().c_str(), "rb");

    if (err != 0) {
        std::cerr << "Couldn't open file " << path << std::endl;
        return;
    }

    if (!file) {
        std::cerr << "File stream is nullptr" << std::endl;
        return;
    }

    // Gonna add comments here so i know what i'm doing in the future
    // This shit is the signature check
    unsigned char sig[8];
  
    fread(sig, 1, 8, file);
    if (!png_check_sig(sig, 8)) {
        std::cerr << "Bad PNG signature" << std::endl;
        fclose(file);
        return;
    }

    // This shit creates the png structs and handles errors
    
    auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        std::cerr << "Couldn't create PNG struct" << std::endl;
        fclose(file);
        return;
    }
  
    this->m_png = png;

    this->m_info = png_create_info_struct(png);
    if (!this->m_info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        std::cerr << "Couldn't create info struct" << std::endl;
        fclose(file);
        return;
    }
    
    // Idk this is in the tutorial, don't ask me what this does. It just checks for errors
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &this->m_info, nullptr);
        std::cerr << "An Error occurred" << std::endl;
        fclose(file);
        return;
    }

    // Finally getting to the actual reading part
    png_init_io(png, file);
    png_set_sig_bytes(png, 8);
    png_read_info(png, this->m_info);
    
    this->readDetails();
    this->readPixels();

    png_destroy_read_struct(&png, &this->m_info, nullptr);

    err = fclose(file);
    if (err != 0) {
        std::cerr << "Couldn't close file " << path << std::endl;
        return;
    }
}

void PngHandler::writePixels() {
    auto const width = this->m_width;
    auto const height = this->m_height;

    auto byteRows = new png_byte*[height];

    for (int i = 0; i < height; ++i) {
        byteRows[i] = new png_byte[width * 4];
    }

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            auto const& pixel = this->m_rows[y][x];

            byteRows[y][x * 4 + 0] = pixel.r;
            byteRows[y][x * 4 + 1] = pixel.g;
            byteRows[y][x * 4 + 2] = pixel.b;
            byteRows[y][x * 4 + 3] = pixel.a;
        }
    }

    png_write_image(this->m_png, byteRows);

    for (int i = 0; i < height; ++i) {
        delete[] byteRows[i];
    }

    delete[] byteRows;
}

void PngHandler::write(std::vector<std::vector<Pixel>> const& pixelVector) {
    this->m_rows = pixelVector;
    this->writePipeline();
}
void PngHandler::write(std::vector<std::vector<Pixel>>&& pixelVector) {
    this->m_rows = std::move(pixelVector);
    this->writePipeline();
}

void PngHandler::writePipeline() {
    if (this->m_rows.size() == 0) {
        std::cerr << "Pixel vector is empty" << std::endl;
        return;
    }

    auto const& path = this->m_path;

    FILE* file;

    errno_t err = fopen_s(&file, path.string().c_str(), "wb");

    if (err != 0) {
        std::cerr << "Couldn't open file " << path << std::endl;
        return;
    }

    if (!file) {
        std::cerr << "File stream is nullptr" << std::endl;
        return;
    }

    // Gonna add comments here so i know what i'm doing in the future
    // This shit creates the png structs and handles errors
    auto png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        std::cerr << "Couldn't create PNG struct" << std::endl;
        fclose(file);
        return;
    }
    this->m_png = png;
    
    auto info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, &info);
        std::cerr << "Couldn't create info struct" << std::endl;
        fclose(file);
        return;
    }
    this->m_info = info;
    
    // Idk this is in the tutorial, don't ask me what this does. It just checks for errors
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        std::cerr << "An Error occurred" << std::endl;
        fclose(file);
        return;
    }

    // Finally getting to the actual writing part
    png_init_io(png, file);

    auto const width = this->m_rows[0].size();
    auto const height = this->m_rows.size();

    this->m_width = width;
    this->m_height = height;

    png_set_IHDR(
        png, info, 
        width, height,
        8, 
        PNG_COLOR_TYPE_RGBA, 
        PNG_INTERLACE_NONE, 
        PNG_COMPRESSION_TYPE_DEFAULT, 
        PNG_FILTER_TYPE_DEFAULT
    );
    
    png_write_info(png, info);
    
    this->writePixels();

    png_write_end(png, nullptr);

    png_destroy_write_struct(&png, &info);
    err = fclose(file);
    if (err != 0) {
        std::cerr << "Couldn't close file " << path << std::endl;
        return;
    }
}