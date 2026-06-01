cmake_minimum_required(VERSION 4.3.2)


project(Idk)

message(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

include_directories(${PROJECT_NAME}
    include
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/FileHandler.cpp
    src/MapProcessor.cpp
)
