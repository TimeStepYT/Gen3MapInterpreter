#pragma once

#include <filesystem>
#include <png.h>

class PngHandler {
    static inline png_infop s_infoPtr = nullptr;
    static inline png_bytepp s_rows = nullptr;

    std::filesystem::path m_path;
public:
    PngHandler(std::filesystem::path const& path);
    void read();
    void write();
};