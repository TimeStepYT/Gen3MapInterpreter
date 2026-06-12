#pragma once

#include <filesystem>

namespace global {
    inline std::filesystem::path g_rootPath{"../../"};
    inline std::filesystem::path g_outputPath{};
    inline bool g_noPng = false;
    inline bool g_noInfo = false;
    inline bool g_renderAll = false;
}