#include <vector>
#include <cstring>
#include <iostream>

#include <FileHandler.hpp>
#include <MapProcessor.hpp>
#include <PngHandler.hpp>
#include <Palette.hpp>

std::string layoutID;
std::filesystem::path rootPath{"../../"};
int width = 8;
bool simple = false;
bool showMetatileInfo = false;

void readArgument(char** argv, int argc, int i, char const* name, int& var) {
    if (strcmp(argv[i], name) != 0)
        return;

    if (i + 1 != argc) {
        var = atoi(argv[i+1]);
    }
}

void readArgument(char** argv, int argc, int i, char const* name, bool& var) {
    if (strcmp(argv[i], name) != 0) 
        return;

    var = true;
}

void readArgument(char** argv, int argc, int i, char const* name, std::filesystem::path& var) {
    if (strcmp(argv[i], name) != 0) 
        return;

    if (i + 1 != argc) {
        var = argv[i+1];
    }
}

void parseArguments(int argc, char** argv) {
    layoutID = argv[1];

    for (int i = 2; i < argc; ++i) {
        auto* arg = argv[i];

        readArgument(argv, argc, i, "-root", rootPath);
        readArgument(argv, argc, i, "-simple", simple);
        readArgument(argv, argc, i, "-metatiles", showMetatileInfo);
    }
}

void handleFileContent(FileHandler const& fileHandler) {
    MapProcessor mapProcessor;
    mapProcessor.showMetatileInfo(showMetatileInfo);
    mapProcessor.simpleMode(simple);

    auto bytes = std::make_unique<std::vector<uint16_t>>();

    bool isFirstPart = true;
    uint8_t firstPart = 0;
    for (uint8_t byte : fileHandler.getBinaryContent()) {
        if (isFirstPart) {
            firstPart = byte;
        }
        else {
            uint16_t resByte = byte << 8;
            resByte += firstPart;

            bytes->emplace_back(resByte);
        }

        isFirstPart = !isFirstPart;
    }

    mapProcessor.processBytes(bytes, width);
    mapProcessor.printData();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::puts("Syntax: Gen3MapInterpreter.exe <layout ID> [OPTIONS]");
        std::puts("    -simple         Only shows the collision data");
        std::puts("    -metatiles      Only show metatile info");
        std::puts("    -root           Set the root directory for the Pokémon Emerald decomp");

        return 0;
    }
    
    parseArguments(argc, argv);

    FileHandler jsonFileHandler;
    jsonFileHandler.setReadErrorMessage("Please provide the root directory with -root");
    jsonFileHandler.readJsonFile(rootPath / "data/layouts/layouts.json");

    auto const& json = jsonFileHandler.getJsonContent();
    auto layouts = json["layouts"];

    nlohmann::json layoutInfo;
    bool found = false;
    for (auto const& layout : layouts) {
        if (layout["id"] != layoutID)
            continue;

        found = true;
        layoutInfo = layout;
        break;
    }

    if (!found) {
        std::cout << "Couldn't find anything with the layout ID \"" << layoutID << '\"';
        return 0;
    }

    width = layoutInfo["width"];

    FileHandler fileHandler;
    bool successReading = fileHandler.readBinaryFile(rootPath / layoutInfo["blockdata_filepath"]);
    if (!successReading)
        return 0;
    
    handleFileContent(fileHandler);

    // if (!simple)
    //     getchar();

    return 0;
}