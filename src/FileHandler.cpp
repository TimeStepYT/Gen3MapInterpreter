#include <FileHandler.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void FileHandler::printFileNotHereError(std::filesystem::path filePath) {
    if (this->m_errorMessage.size() != 0) {
        std::cerr << this->m_errorMessage << std::endl;
        return;
    }

    std::cerr << "The file " << filePath << " doesn't exist!" << std::endl;
}

bool FileHandler::readBinaryFile(std::filesystem::path const& filePath) {
    if (!std::filesystem::exists(filePath)) {
        this->printFileNotHereError(filePath);
        return false;
    }

    std::ifstream file(filePath, std::ios_base::binary);

    auto fileSize = std::filesystem::file_size(filePath);
    this->m_fileSize = fileSize;
    this->m_binaryContent.reserve(fileSize);

    char ch;
    size_t i = 0;

    while (file.read(&ch, 1)) {
        this->m_binaryContent.push_back(ch);
        ++i;
    }

    this->m_hasRead = true;

    return true;
}

bool FileHandler::readJsonFile(std::filesystem::path const& filePath) {
    if (!std::filesystem::exists(filePath)) {
        this->printFileNotHereError(filePath);
        return false;
    }

    std::ifstream file(filePath);

    auto const& fileSize = std::filesystem::file_size(filePath);
    this->m_fileSize = fileSize;

    json data = json::parse(file);

    this->m_jsonContent = std::move(data);

    this->m_hasRead = true;

    return true;
}

json const& FileHandler::getJsonContent() const{
    return this->m_jsonContent;
}

std::vector<uint8_t> const& FileHandler::getBinaryContent() const {
    if (!this->m_hasRead) {
        std::cerr << "ERROR: The file wasn't read yet!" << std::endl;
    }

    return this->m_binaryContent;
}

uintmax_t FileHandler::getSize() {
    if (!this->m_hasRead) {
        std::cerr << "ERROR: The file wasn't read yet!" << std::endl;
        return 0;
    }
    return this->m_fileSize;
}

void FileHandler::setReadErrorMessage(std::string const& errorMessage) {
    this->m_errorMessage = errorMessage;
}