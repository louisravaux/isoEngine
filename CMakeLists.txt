cmake_minimum_required(VERSION 3.16)
project(isoEngine)

#Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the output directory for built objects.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in SDL
add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(SDL_image EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(isoEngine 
    src/main.cpp
    src/core/Engine.cpp
    src/core/Map.cpp
    src/core/Tile.cpp
    src/utils/Math.cpp
)

# Configurer les répertoires d'inclusion
target_include_directories(isoEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link to the actual SDL3 library.
target_link_libraries(isoEngine PRIVATE SDL3_image::SDL3_image SDL3::SDL3)