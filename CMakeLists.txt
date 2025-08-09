cmake_minimum_required(VERSION 3.16)
project(isoEngine)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for built objects
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# SDL3
add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(SDL_image EXCLUDE_FROM_ALL)

# ImGui
set(IMGUI_SOURCES
    ImGui/imgui.cpp
    ImGui/imgui_demo.cpp
    ImGui/imgui_draw.cpp
    ImGui/imgui_tables.cpp
    ImGui/imgui_widgets.cpp
    ImGui/backends/imgui_impl_sdl3.cpp
    ImGui/backends/imgui_impl_sdlrenderer3.cpp
)

# Create ImGui as a static library
add_library(ImGui STATIC ${IMGUI_SOURCES})

# Set include directories for ImGui library
target_include_directories(ImGui PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/backends
)

# Link SDL3 to ImGui
target_link_libraries(ImGui PUBLIC SDL3::SDL3)

# Create your game executable target
add_executable(isoEngine 
    src/main.cpp
    src/core/Engine.cpp
    src/core/Map.cpp
    src/core/Tile.cpp
    src/utils/Math.cpp
    src/UI/UIManager.cpp
    src/UI/UIDebug.cpp
)

# Include directories for your game
target_include_directories(isoEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries to your executable
target_link_libraries(isoEngine PRIVATE SDL3_image::SDL3_image SDL3::SDL3 ImGui)