#include <PngHandler.hpp>

#include <iostream>
#include <stdio.h>

PngHandler::PngHandler(std::filesystem::path const& path) {
    this->m_path = path;
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

    if (this->m_colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
        png_read_update_info(png, info);
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
    this->m_rows.reserve(this->m_height);

    for (int y = 0; y < this->m_height; ++y) {
        std::vector<png_byte> newVec(rows[y], rows[y] + rowSize);

        this->m_rows.emplace_back(std::move(newVec));
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
        return;
    }

    // This shit creates the png structs and handles errors
    
    auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        std::cerr << "Couldn't create PNG struct" << std::endl;
        return;
    }
  
    this->m_png = png;

    this->m_info = png_create_info_struct(png);
    if (!this->m_info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        std::cerr << "Couldn't create info struct" << std::endl;
        return;
    }
    
    // Idk this is in the tutorial, don't ask me what this does. It just checks for errors
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &this->m_info, nullptr);
        std::cerr << "An Error occurred" << std::endl;
        return;
    }

    // Finally getting to the actual reading part
    png_init_io(png, file);
    png_set_sig_bytes(png, 8);
    png_read_info(png, this->m_info);
    
    this->readDetails();
    this->readPixels();

    png_destroy_read_struct(&png, &this->m_info, nullptr);


    auto px = this->m_rows[0];

    std::cout << "First Pixel: " << static_cast<int>(px[0]) << ' ' << static_cast<int>(px[1]) << ' ' << static_cast<int>(px[2]) << std::endl;

    
    err = fclose(file);
    if (err != 0) {
        std::cerr << "Couldn't close file " << path << std::endl;
        return;
    }
}

void PngHandler::write() {
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

    //TODO

    err = fclose(file);

    if (err != 0) {
        std::cerr << "Couldn't close file " << path << std::endl;
        return;
    }
}