#pragma once

#include <filesystem>
#include <vector>
#include <nlohmann/json.hpp>

class FileHandler {
    std::vector<uint8_t> m_binaryContent{};
    nlohmann::json m_jsonContent;
    bool m_hasRead = false;
    uintmax_t m_fileSize{};
    std::string m_errorMessage{""};

    void printFileNotHereError(std::filesystem::path filePath);

public:

    std::unique_ptr<std::vector<uint16_t>> getU16Vector() const;
    bool readBinaryFile(std::filesystem::path const&);
    bool readJsonFile(std::filesystem::path const&);
    std::vector<uint8_t> const& getBinaryContent() const;
    nlohmann::json const& getJsonContent() const;
    void setReadErrorMessage(std::string const& errorMessage);
    uintmax_t getSize();
};