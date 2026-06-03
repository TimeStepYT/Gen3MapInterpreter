cmake_minimum_required(VERSION 3.28.3)

project(Gen3MapInterpreter)

message("Configuring project: ${PROJECT_NAME}")

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)

CPMAddPackage(gh:madler/zlib@1.3.2)
CPMAddPackage(gh:pnggroup/libpng@1.6.58)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/FileHandler.cpp
    src/MapProcessor.cpp
    src/MapMetatile.cpp
    src/PngHandler.cpp
    src/Palette.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    zlibstatic
    png_static
)