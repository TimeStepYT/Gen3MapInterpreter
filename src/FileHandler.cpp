#include <FileHandler.hpp>
#include <fstream>
#include <iostream>

bool FileHandler::readFile(std::filesystem::path const& filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cout << "The file " << filePath << " doesn't exist!" << std::endl;
        return false;
    }

    std::ifstream file(filePath, std::ios_base::binary);

    auto fileSize = std::filesystem::file_size(filePath);
    this->m_fileSize = fileSize;
    this->m_fileContent.reserve(fileSize);

    char ch;
    size_t i = 0;

    while (file.read(&ch, 1)) {
        this->m_fileContent.push_back(ch);
        ++i;
    }

    this->m_hasRead = true;

    return true;
}

std::vector<uint8_t> const& FileHandler::getContent() {
    if (!this->m_hasRead) {
        std::cerr << "ERROR: The file wasn't read yet!" << std::endl;
    }

    return this->m_fileContent;
}

uintmax_t FileHandler::getSize() {
    if (!this->m_hasRead) {
        std::cerr << "ERROR: The file wasn't read yet!" << std::endl;
        return 0;
    }
    return this->m_fileSize;
}