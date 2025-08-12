#include "UI/UIDebug.hpp"
#include "core/Engine.hpp"

#include "imgui.h"

UIDebug::UIDebug(IsoEngine *engineRef) : engine(engineRef) {
    
}

UIDebug::~UIDebug() {
}

void UIDebug::content() {
    if (!engine) return;

    ImGui::SetNextWindowSize(ImVec2(250, 350), ImGuiCond_FirstUseEver);
    
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

    ImGui::Begin("Debug Info");

    // FPS info display
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame Time: %.3f ms", ImGui::GetIO().DeltaTime * 1000.0f);

    // Frame Time Profiler
    static float frameTimeHistory[100] = {};
    static int frameTimeIndex = 0;
    frameTimeHistory[frameTimeIndex] = ImGui::GetIO().DeltaTime * 1000.0f;
    frameTimeIndex = (frameTimeIndex + 1) % IM_ARRAYSIZE(frameTimeHistory);
    ImGui::PlotLines("Frame Time", frameTimeHistory, IM_ARRAYSIZE(frameTimeHistory), frameTimeIndex, nullptr, 0.0f, 100.0f, ImVec2(0, 60));

    ImGui::Separator();

    // Window size
    int windowWidth, windowHeight;
    SDL_GetWindowSize(engine->getWindow(), &windowWidth, &windowHeight);
    ImGui::Text("Window Size: %dx%d", windowWidth, windowHeight);

    ImGui::Text("Mouse: (%d, %d)", engine->mouseX, engine->mouseY);

    // Tile info display
    if (engine->selectedTileX >= 0 && engine->selectedTileY >= 0) {
        Tile* selectedTile = engine->gameMaps[engine->activeMapIndex]->getTile(engine->selectedTileX, engine->selectedTileY, engine->selectedLayer);
        if (selectedTile) {
            ImGui::Text("Tile ID: %d", selectedTile->getID());
            ImGui::Text("Grid Pos: (%d, %d)", selectedTile->getGridX(), selectedTile->getGridY());
            ImGui::Text("Screen Pos: (%d, %d)", selectedTile->getScreenX(), selectedTile->getScreenY());
        } else {
            ImGui::Text("No tile at selected position");
        }
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No tile selected");
    }

    if(ImGui::CollapsingHeader("Map Info", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Active Map: %d", engine->activeMapIndex);
        // change map
        if (ImGui::Button("Next Map")) {
            engine->activeMapIndex = (engine->activeMapIndex + 1) % engine->gameMaps.size();
        }
        ImGui::Text("Map Size: %dx%d", engine->gameMaps[engine->activeMapIndex]->getWidth(), engine->gameMaps[engine->activeMapIndex]->getHeight());
        ImGui::Text("Layers: %d", engine->gameMaps[engine->activeMapIndex]->getLayerCount());
        ImGui::Text("Current Layer: %d", engine->selectedLayer);
        //change layer button
        if (ImGui::Button("Change Layer")) {
            engine->selectedLayer = (engine->selectedLayer + 1) % engine->gameMaps[engine->activeMapIndex]->getLayerCount();
        }
    }

    if (ImGui::CollapsingHeader("Tile Types", ImGuiTreeNodeFlags_DefaultOpen)) {
        static int selectedID = -1; // Keeps track of currently selected tile type

        for (const auto& tile : TileRegistry::getAllTypes()) {
            int id = TileRegistry::getTileID(tile);
            std::string label = tile->getName() + "##" + std::to_string(id);
            // "##" avoids ImGui ID conflicts when names repeat

            // Highlight the selected one
            if (ImGui::Selectable(label.c_str(), selectedID == id)) {
                selectedID = id;
                engine->selectedTileType = id;
            }

            SDL_Texture* tex = tile->getTexture();
            if (tex) {
                ImVec2 size(16, 16); // preview size
                ImGui::SameLine();
                ImGui::Image((ImTextureID)tex, size);
            }
        }
    }


    // Camera controls
    if (ImGui::CollapsingHeader("Camera")) {
        float camX = engine->gameMaps[engine->activeMapIndex]->getCameraX();
        float camY = engine->gameMaps[engine->activeMapIndex]->getCameraY();

        if (ImGui::SliderFloat("Camera X", &camX, -1000.0f, 1000.0f)) {
            engine->gameMaps[engine->activeMapIndex]->setCamera(camX, camY);
        }
        if (ImGui::SliderFloat("Camera Y", &camY, -1000.0f, 1000.0f)) {
            engine->gameMaps[engine->activeMapIndex]->setCamera(camX, camY);
        }
        
        if (ImGui::Button("Reset Camera")) {
            engine->gameMaps[engine->activeMapIndex]->setCamera(-640.0f, -16.0f);
        }
    }
    
    ImGui::End();
}
