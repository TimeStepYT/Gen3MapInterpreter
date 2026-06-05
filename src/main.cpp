#include <vector>
#include <cstring>
#include <iostream>
#include <locale>

#include <global.hpp>
#include <FileHandler.hpp>
#include <MapProcessor.hpp>
#include <PngHandler.hpp>
#include <Palette.hpp>

std::string layoutID;
int width = 8;
bool simple = false;
bool showMetatileInfo = false;

nlohmann::json layoutInfo;

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

        readArgument(argv, argc, i, "-root", global::g_rootPath);
        readArgument(argv, argc, i, "-simple", simple);
        readArgument(argv, argc, i, "-metatiles", showMetatileInfo);
        readArgument(argv, argc, i, "-o", global::g_outputPath);
    }
}

bool findLayoutInfo() {
    FileHandler jsonFileHandler;
    jsonFileHandler.setReadErrorMessage("Please provide the root directory with -root");
    jsonFileHandler.readJsonFile(global::g_rootPath / "data/layouts/layouts.json");

    auto const& json = jsonFileHandler.getJsonContent();
    auto layouts = json["layouts"];

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
    return true;
}

void handleBlockDataFileContent(FileHandler const& fileHandler) {
    MapProcessor mapProcessor;
    mapProcessor.setTilesets(layoutInfo["primary_tileset"], layoutInfo["secondary_tileset"]);
    mapProcessor.showMetatileInfo(showMetatileInfo);
    mapProcessor.simpleMode(simple);

    auto bytes = fileHandler.getU16Vector();

    mapProcessor.processBytes(bytes, width);
    // mapProcessor.printData();
    mapProcessor.renderMap(global::g_outputPath);
}

int main(int argc, char** argv) {
    std::locale::global(std::locale("en_US.UTF-8"));

    if (argc < 2) {
        std::puts("Syntax: Gen3MapInterpreter.exe <layout ID> [OPTIONS]");
        std::puts("    -simple         Only shows the collision data");
        std::puts("    -metatiles      Only show metatile info");
        std::puts("    -root           Set the root directory for the Pokémon Emerald decomp");

        return 0;
    }
    
    parseArguments(argc, argv);

    if (!findLayoutInfo())
        return 0;

    width = layoutInfo["width"];

    FileHandler fileHandler;
    bool successReading = fileHandler.readBinaryFile(global::g_rootPath / layoutInfo["blockdata_filepath"]);
    if (!successReading)
        return 0;
    
    handleBlockDataFileContent(fileHandler);

    // if (!simple)
    //     getchar();

    return 0;
}