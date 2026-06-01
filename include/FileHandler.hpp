#pragma once

#include <filesystem>
#include <vector>

class FileHandler {
    std::vector<uint8_t> m_fileContent{};
    bool m_hasRead = false;
    uintmax_t m_fileSize{};

public:

    bool readFile(std::filesystem::path const&);
    std::vector<uint8_t> const& getContent();
    uintmax_t getSize();
};