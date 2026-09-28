#include <vector>
#include <cstring>
#include <fmt/core.h>
#include <locale>

#include <global.hpp>
#include <FileHandler.hpp>
#include <MapProcessor.hpp>
#include <PngHandler.hpp>
#include <Palette.hpp>
#include <Profiler.hpp>

std::string g_layoutID;
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

    std::string pathString = argv[i+1];

    pathString.erase(std::remove(pathString.begin(), pathString.end(), '\"'), pathString.end());
    pathString.erase(std::remove(pathString.begin(), pathString.end(), '\''), pathString.end());

    if (i + 1 != argc) {
        var = pathString;
    }
}

void parseArguments(int argc, char** argv) {
    for (int i = 2; i < argc; ++i) {
        auto* arg = argv[i];

        readArgument(argv, argc, i, "-root", global::g_rootPath);
        readArgument(argv, argc, i, "-simple", simple);
        readArgument(argv, argc, i, "-metatiles", showMetatileInfo);
        readArgument(argv, argc, i, "-o", global::g_outputPath);
        readArgument(argv, argc, i, "-nopng", global::g_noPng);
        readArgument(argv, argc, i, "-noinfo", global::g_noInfo);
        readArgument(argv, argc, i, "-renderAll", global::g_renderAll);
        readArgument(argv, argc, i, "-silent", global::g_silent);
    }
    std::ostringstream layoutID;
    layoutID << "LAYOUT_" << argv[1];

    g_layoutID = layoutID.str();
}

bool findLayoutInfo() {
    FileHandler jsonFileHandler;
    jsonFileHandler.setReadErrorMessage("Please provide the root directory with -root");
    jsonFileHandler.readJsonFile(global::g_rootPath / "data/layouts/layouts.json");

    auto const& json = jsonFileHandler.getJsonContent();
    auto layouts = json["layouts"];

    bool found = false;
    for (auto const& layout : layouts) {
        if (layout["id"] != g_layoutID)
            continue;

        found = true;
        layoutInfo = layout;
        break;
    }

    if (!found) {
        fmt::println("Couldn't find anything with the layout ID \"{}\"", g_layoutID);
        return 0;
    }
    return true;
}

void handleBlockDataFileContent(FileHandler const& fileHandler) {
    MapProcessor mapProcessor;
    mapProcessor.setTilesets(layoutInfo["primary_tileset"], layoutInfo["secondary_tileset"]);
    mapProcessor.showMetatileInfo(showMetatileInfo);
    mapProcessor.simpleMode(simple);

    std::string nameRaw = layoutInfo["name"];
    auto name = nameRaw.substr(0, nameRaw.size() - 7);

    mapProcessor.setName(name);

    auto bytes = fileHandler.getU16Vector();

    mapProcessor.processBytes(bytes, width);
    
    
    if (!global::g_noInfo)
    mapProcessor.printData();
    
    if (!global::g_noPng)
        mapProcessor.renderMap(global::g_outputPath);
}

void handleLayoutInfo() {
    width = layoutInfo["width"];

    FileHandler fileHandler;
    bool successReading = fileHandler.readBinaryFile(global::g_rootPath / layoutInfo["blockdata_filepath"]);
    if (!successReading)
        return;
    
    handleBlockDataFileContent(fileHandler);
}

void renderEverything() {
    FileHandler jsonFileHandler;
    jsonFileHandler.setReadErrorMessage("Please provide the root directory with -root");
    jsonFileHandler.readJsonFile(global::g_rootPath / "data/layouts/layouts.json");

    auto const& json = jsonFileHandler.getJsonContent();
    auto layouts = json["layouts"];

    for (auto const& layout : layouts) {
        layoutInfo = layout;
        handleLayoutInfo();
    }
}

int main(int argc, char** argv) {
#ifdef _WIN32
    std::locale::global(std::locale("en_US.UTF-8"));
#endif
    if (argc < 2) {
        fmt::println("Syntax: Gen3MapInterpreter.exe <layout ID> [OPTIONS]");
        fmt::println("    -simple            Only shows the collision data");
        fmt::println("    -metatiles         Only show metatile info");
        fmt::println("    -nopng             Doesn't create a PNG of the map");
        fmt::println("    -noinfo            Doesn't log the layout data");
        fmt::println("    -root <directory>  Set the root directory for the Pokémon Emerald decomp");
        fmt::println("    -o <directory>     Set the output directory for the exported PNG");
        fmt::println("    -renderAll         Render every single map in the game");
        fmt::println("    -silent            Disable log for exporting a png");

        return 0;
    }
    
    parseArguments(argc, argv);

    if (global::g_noInfo && global::g_noPng) {
        fmt::println("Literally not doing anything but wasting your time :D");
        // Not returning out of spite.
    }

    if (global::g_renderAll) {
        global::g_noInfo = true;
        global::g_noPng = false;
        Profiler p;
        renderEverything();
        p.printSeconds("All maps");
        return 0;
    }

    if (!findLayoutInfo())
        return 0;

    handleLayoutInfo();

    // if (!simple)
    //     getchar();

    return 0;
}