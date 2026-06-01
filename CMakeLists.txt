cmake_minimum_required(VERSION 4.3.2)

project(Gen3MapInterpreter)

message(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

include_directories(${PROJECT_NAME}
    include
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/FileHandler.cpp
    src/MapProcessor.cpp
    src/MapMetatile.cpp
    src/PngHandler.cpp
)

include(cmake/CPM.cmake)

CPMAddPackage(gh:madler/zlib@1.3.2)
CPMAddPackage(gh:pnggroup/libpng@1.6.58)

target_link_libraries(${PROJECT_NAME} PRIVATE zlib png_shared)