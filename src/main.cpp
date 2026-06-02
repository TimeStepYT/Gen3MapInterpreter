#include <vector>
#include <cstring>

#include <FileHandler.hpp>
#include <MapProcessor.hpp>
#include <PngHandler.hpp>
#include <Palette.hpp>

std::filesystem::path filePath;
int width = 8;
FileHandler fileHandler;
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

void parseArguments(int argc, char** argv) {
    filePath = argv[1];

    for (int i = 2; i < argc; ++i) {
        auto* arg = argv[i];

        readArgument(argv, argc, i, "-width", width);
        readArgument(argv, argc, i, "-simple", simple);
        readArgument(argv, argc, i, "-metatiles", showMetatileInfo);
    }
}

void handleFileContent() {
    MapProcessor mapProcessor;
    mapProcessor.showMetatileInfo(showMetatileInfo);
    mapProcessor.simpleMode(simple);

    auto bytes = std::make_unique<std::vector<uint16_t>>();

    bool isFirstPart = true;
    uint8_t firstPart = 0;
    for (uint8_t byte : fileHandler.getContent()) {
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
        std::puts("Syntax: Gen3MapInterpreter.exe <filepath> [OPTIONS]");
        std::puts("    -simple         Only shows the collision data");
        std::puts("    -metatiles      Only show metatile info");
        std::puts("    -width <value>  Set the display width");

        return 0;
    }

    parseArguments(argc, argv);

    bool successReading = fileHandler.readFile(filePath);
    if (!successReading)
        return 0;
    
    handleFileContent();

    // if (!MapProcessor::m_simple)
    //     getchar();

    return 0;
}