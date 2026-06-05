cmake_minimum_required(VERSION 3.28.3)

project(Gen3MapInterpreter)

message("Configuring project: ${PROJECT_NAME}")

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)

CPMAddPackage(gh:madler/zlib@1.3.2)

if (zlib_SOURCE_DIR)
    set(ZLIB_INCLUDE_DIRS "${zlib_SOURCE_DIR}")
endif()

CPMAddPackage(gh:pnggroup/libpng@1.6.58)
CPMAddPackage(gh:nlohmann/json@3.12.0)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/FileHandler.cpp
    src/MapProcessor.cpp
    src/LayoutMetatile.cpp
    src/PngHandler.cpp
    src/Palette.cpp
    src/Tileset.cpp
    src/Metatile.cpp
    src/Tile.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    zlibstatic
    png_static
    nlohmann_json
)