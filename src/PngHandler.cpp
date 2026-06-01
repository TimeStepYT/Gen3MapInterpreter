#include <PngHandler.hpp>

#include <iostream>
#include <stdio.h>

PngHandler::PngHandler(std::filesystem::path const& path) {
    this->m_path = path;
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

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    this->s_infoPtr = png_create_info_struct(png_ptr);  
    png_init_io(png_ptr, file);
    png_read_png(png_ptr, this->s_infoPtr, PNG_TRANSFORM_IDENTITY, NULL);
    this->s_rows = png_get_rows(png_ptr, this->s_infoPtr);
    png_destroy_read_struct(&png_ptr, NULL, NULL); 

    err = fclose(file);

    if (err != 0) {
        std::cerr << "Couldn't close file" << path << std::endl;
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

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_init_io(png_ptr, file);
    png_set_rows(png_ptr, this->s_infoPtr, this->s_rows);
    png_write_png(png_ptr, this->s_infoPtr, PNG_TRANSFORM_IDENTITY, NULL);
    png_destroy_write_struct(&png_ptr, &this->s_infoPtr);

    err = fclose(file);

    if (err != 0) {
        std::cerr << "Couldn't close file" << path << std::endl;
        return;
    }
}