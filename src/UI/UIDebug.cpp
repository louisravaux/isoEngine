#include "UIDebug.hpp"
#include "core/Engine.hpp"

#include "imgui.h"

UIDebug::UIDebug(IsoEngine *engineRef) : engine(engineRef) {
    
}

UIDebug::~UIDebug() {
}

void UIDebug::content() {
    if (!engine) return;

    ImGui::SetNextWindowSize(ImVec2(170, 220), ImGuiCond_FirstUseEver);
    
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

    ImGui::Begin("Debug Info");
    
    ImGui::Text("Mouse: (%d, %d)", engine->mouseX, engine->mouseY);

    // Tile info display
    if (engine->selectedTileX >= 0 && engine->selectedTileY >= 0) {
        Tile* selectedTile = engine->gameMap->getTile(engine->selectedTileX, engine->selectedTileY);
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
        float camX = engine->gameMap->getCameraX();
        float camY = engine->gameMap->getCameraY();
        
        if (ImGui::SliderFloat("Camera X", &camX, -1000.0f, 1000.0f)) {
            engine->gameMap->setCamera(camX, camY);
        }
        if (ImGui::SliderFloat("Camera Y", &camY, -1000.0f, 1000.0f)) {
            engine->gameMap->setCamera(camX, camY);
        }
        
        if (ImGui::Button("Reset Camera")) {
            engine->gameMap->setCamera(-640.0f, -16.0f);
        }
    }
    
    ImGui::End();
}
