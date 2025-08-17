#include "UI/UIDebug.hpp"
#include "core/Engine.hpp"
#include "imgui.h"

UIDebug::UIDebug(IsoEngine *engineRef) : engine(engineRef) {
    // Initialize window visibility states
    showPerformanceWindow = true;
    showTileInspector = true;
    showLevelManager = true;
    showTilePalette = true;
    showCameraControls = true;
    showSystemInfo = false; // Start hidden
}

UIDebug::~UIDebug() {
}

void UIDebug::content() {
    if (!engine) return;
    if (isHidden) return;

    // Main menu bar for toggling windows
    drawMainMenuBar();
    
    // Individual window components
    if (showPerformanceWindow) drawPerformanceWindow();
    if (showTileInspector) drawTileInspectorWindow();
    if (showLevelManager) drawLevelManagerWindow();
    if (showTilePalette) drawTilePaletteWindow();
    if (showCameraControls) drawCameraControlsWindow();
    if (showSystemInfo) drawSystemInfoWindow();
}

void UIDebug::drawMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Debug Windows")) {
            ImGui::MenuItem("Performance", "Ctrl+1", &showPerformanceWindow);
            ImGui::MenuItem("Tile Inspector", "Ctrl+2", &showTileInspector);
            ImGui::MenuItem("Level Manager", "Ctrl+3", &showLevelManager);
            ImGui::MenuItem("Tile Palette", "Ctrl+4", &showTilePalette);
            ImGui::MenuItem("Camera Controls", "Ctrl+5", &showCameraControls);
            ImGui::MenuItem("System Info", "Ctrl+6", &showSystemInfo);
            ImGui::Separator();
            if (ImGui::MenuItem("Hide All", "Ctrl+H")) {
                hideAllWindows();
            }
            if (ImGui::MenuItem("Show All", "Ctrl+Shift+H")) {
                showAllWindows();
            }
            ImGui::EndMenu();
        }
        
        // Quick stats in menu bar
        ImGui::Text("| FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("| Frame: %.2fms", ImGui::GetIO().DeltaTime * 1000.0f);
        
        ImGui::EndMainMenuBar();
    }
}

void UIDebug::drawPerformanceWindow() {
    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Performance Monitor", &showPerformanceWindow)) {
        // FPS Display
        ImGui::Text("FPS: %.1f (%.3f ms/frame)", 
                   ImGui::GetIO().Framerate, 
                   ImGui::GetIO().DeltaTime * 1000.0f);
        
        // Frame time history graph
        static float frameTimeHistory[120] = {};
        static int frameTimeIndex = 0;
        static float maxFrameTime = 16.67f; // Start with 60fps baseline
        
        float currentFrameTime = ImGui::GetIO().DeltaTime * 1000.0f;
        frameTimeHistory[frameTimeIndex] = currentFrameTime;
        frameTimeIndex = (frameTimeIndex + 1) % IM_ARRAYSIZE(frameTimeHistory);
        
        // Auto-scale the graph
        if (currentFrameTime > maxFrameTime) {
            maxFrameTime = currentFrameTime * 1.1f;
        }
        
        ImGui::PlotLines("Frame Time (ms)", frameTimeHistory, 
                        IM_ARRAYSIZE(frameTimeHistory), frameTimeIndex, 
                        nullptr, 0.0f, maxFrameTime, ImVec2(0, 80));
        
        // Performance metrics
        ImGui::Separator();
        ImGui::Text("Target: 60 FPS (16.67ms)");
        
        // Color-coded performance status
        if (currentFrameTime < 16.67f) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "GOOD");
        } else if (currentFrameTime < 33.33f) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "OK");
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "POOR");
        }
        
        if (ImGui::Button("Reset Graph")) {
            maxFrameTime = 16.67f;
            memset(frameTimeHistory, 0, sizeof(frameTimeHistory));
        }
    }
    ImGui::End();
}

void UIDebug::drawTileInspectorWindow() {
    ImGui::SetNextWindowSize(ImVec2(280, 250), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(370, 30), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Tile Inspector", &showTileInspector)) {
        Map* currentMap = engine->gameLevels[engine->activeLevelIndex]->getCurrentMap();
        
        ImGui::Text("Mouse Position: (%d, %d)", engine->mouseX, engine->mouseY);
        
        ImGui::Separator();
        
        if (engine->selectedTileX >= 0 && engine->selectedTileY >= 0) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Tile Selected");
            ImGui::Text("Grid Position: (%d, %d)", engine->selectedTileX, engine->selectedTileY);
            
            Tile* selectedTile = currentMap->getTile(engine->selectedTileX, engine->selectedTileY, engine->selectedLayer);
            if (selectedTile) {
                ImGui::Text("Tile ID: %d", selectedTile->getID());
                ImGui::Text("Screen Position: (%d, %d)", selectedTile->getScreenX(), selectedTile->getScreenY());
                ImGui::Text("Layer: %d", engine->selectedLayer);
                
                // Tile preview if texture exists
                SDL_Texture* tex = selectedTile->getTexture();
                if (tex) {
                    ImGui::Separator();
                    ImGui::Text("Preview:");
                    ImVec2 size(64, 64);
                    ImGui::Image((ImTextureID)tex, size);
                }
                
                // Quick tile operations
                ImGui::Separator();
                if (ImGui::Button("Delete Tile")) {
                    currentMap->removeTile(engine->selectedTileX, engine->selectedTileY, engine->selectedLayer);
                }
                ImGui::SameLine();
                if (ImGui::Button("Replace Tile")) {
                    // Replace with currently selected tile type
                    // Implementation would depend on your tile creation system
                }
            } else {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "No tile at position");
            }
        } else {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No tile selected");
            ImGui::Text("Click on a tile to inspect it");
        }
    }
    ImGui::End();
}

void UIDebug::drawLevelManagerWindow() {
    ImGui::SetNextWindowSize(ImVec2(300, 280), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(10, 250), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Level & Map Manager", &showLevelManager)) {
        Map* currentMap = engine->gameLevels[engine->activeLevelIndex]->getCurrentMap();
        
        // Level Management
        ImGui::SeparatorText("Level Management");
        ImGui::Text("Active Level: %d", engine->activeLevelIndex);
        ImGui::Text("Total Levels: %zu", engine->gameLevels.size());
        
        if (ImGui::Button("Previous Level")) {
            if (engine->activeLevelIndex > 0) {
                engine->activeLevelIndex--;
            } else {
                engine->activeLevelIndex = engine->gameLevels.size() - 1;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Next Level")) {
            if (engine->activeLevelIndex + 1 < engine->gameLevels.size()) {
                engine->activeLevelIndex++;
            } else {
                engine->activeLevelIndex = 0;
            }
        }
        
        // Map Management
        ImGui::SeparatorText("Map Management");
        ImGui::Text("Map Size: %dx%d", currentMap->getWidth(), currentMap->getHeight());
        ImGui::Text("Layer Count: %d", currentMap->getLayerCount());
        ImGui::Text("Current Layer: %d", engine->selectedLayer);
        
        // if (ImGui::Button("Previous Map")) {
        //     engine->gameLevels[engine->activeLevelIndex]->previousMap();
        // }
        ImGui::SameLine();
        if (ImGui::Button("Next Map")) {
            engine->gameLevels[engine->activeLevelIndex]->nextMap();
        }
        
        if (ImGui::Button("Previous Layer")) {
            if (engine->selectedLayer > 0) {
                engine->selectedLayer--;
            } else {
                engine->selectedLayer = currentMap->getLayerCount() - 1;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Next Layer")) {
            engine->selectedLayer = (engine->selectedLayer + 1) % currentMap->getLayerCount();
        }
        
        // Background Color
        ImGui::SeparatorText("Map Settings");
        SDL_Color bgColor = currentMap->getBackgroundColor();
        static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // Static to persist between frames
        
        // Update static color array with current background color
        color[0] = bgColor.r / 255.0f;
        color[1] = bgColor.g / 255.0f;
        color[2] = bgColor.b / 255.0f;
        color[3] = bgColor.a / 255.0f;
        
        if (ImGui::ColorEdit4("Background Color", color, ImGuiColorEditFlags_NoInputs)) {
            SDL_Color newColor;
            newColor.r = (Uint8)(color[0] * 255.0f);
            newColor.g = (Uint8)(color[1] * 255.0f);
            newColor.b = (Uint8)(color[2] * 255.0f);
            newColor.a = (Uint8)(color[3] * 255.0f);
            currentMap->setBackgroundColor(newColor);
        }
    }
    ImGui::End();
}

void UIDebug::drawTilePaletteWindow() {
    ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(660, 30), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Tile Palette", &showTilePalette)) {
        ImGui::Text("Selected Type: %d", engine->selectedTileType);
        ImGui::Separator();
        
        // Search filter
        static char searchBuffer[128] = "";
        ImGui::InputText("Search", searchBuffer, sizeof(searchBuffer));
        
        ImGui::Separator();
        
        // Tile grid view
        const float buttonSize = 40.0f;
        const float spacing = 2.0f;
        const int tilesPerRow = (int)((ImGui::GetContentRegionAvail().x - spacing) / (buttonSize + spacing));
        
        int tileCount = 0;
        for (const auto& tile : TileRegistry::getAllTypes()) {
            int id = TileRegistry::getTileID(tile);
            std::string name = tile->getName();
            
            // Filter by search
            if (strlen(searchBuffer) > 0) {
                if (name.find(searchBuffer) == std::string::npos && 
                    std::to_string(id).find(searchBuffer) == std::string::npos) {
                    continue;
                }
            }
            
            // Grid layout
            if (tileCount > 0 && tileCount % tilesPerRow != 0) {
                ImGui::SameLine();
            }
            
            // Tile button with preview
            bool isSelected = (engine->selectedTileType == id);
            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 1.0f, 1.0f));
            }
            
            if (ImGui::Button(("##" + std::to_string(id)).c_str(), ImVec2(buttonSize, buttonSize))) {
                engine->selectedTileType = id;
            }
            
            if (isSelected) {
                ImGui::PopStyleColor();
            }
            
            // Texture preview on button
            SDL_Texture* tex = tile->getTexture();
            if (tex) {
                ImVec2 buttonMin = ImGui::GetItemRectMin();
                ImVec2 buttonMax = ImGui::GetItemRectMax();
                ImGui::GetWindowDrawList()->AddImage((ImTextureID)tex, buttonMin, buttonMax);
            }
            
            // Tooltip with details
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("ID: %d", id);
                ImGui::Text("Name: %s", name.c_str());
                ImGui::EndTooltip();
            }
            
            tileCount++;
        }
    }
    ImGui::End();
}

void UIDebug::drawCameraControlsWindow() {
    ImGui::SetNextWindowSize(ImVec2(280, 180), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(320, 250), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Camera Controls", &showCameraControls)) {
        Map* currentMap = engine->gameLevels[engine->activeLevelIndex]->getCurrentMap();
        
        float camX = currentMap->getCameraX();
        float camY = currentMap->getCameraY();
        
        ImGui::Text("Position: (%.1f, %.1f)", camX, camY);
        
        // Precise controls
        if (ImGui::DragFloat("Camera X", &camX, 1.0f, -5000.0f, 5000.0f)) {
            currentMap->setCamera(camX, camY);
        }
        if (ImGui::DragFloat("Camera Y", &camY, 1.0f, -5000.0f, 5000.0f)) {
            currentMap->setCamera(camX, camY);
        }
        
        // Quick movement buttons
        ImGui::Separator();
        const float step = 32.0f;
        
        if (ImGui::Button("Up")) currentMap->moveCamera(0, -step);
        ImGui::SameLine();
        if (ImGui::Button("Down")) currentMap->moveCamera(0, step);
        ImGui::SameLine();
        if (ImGui::Button("Left")) currentMap->moveCamera(-step, 0);
        ImGui::SameLine();
        if (ImGui::Button("Right")) currentMap->moveCamera(step, 0);
        
        ImGui::Separator();
        if (ImGui::Button("Center Camera")) {
            currentMap->setCamera(-640.0f, -16.0f);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset to Origin")) {
            currentMap->setCamera(0.0f, 0.0f);
        }
    }
    ImGui::End();
}

void UIDebug::drawSystemInfoWindow() {
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(620, 250), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("System Information", &showSystemInfo)) {
        // Window information
        int windowWidth, windowHeight;
        SDL_GetWindowSize(engine->getWindow(), &windowWidth, &windowHeight);
        ImGui::Text("Window Size: %dx%d", windowWidth, windowHeight);
        
        // Memory usage (if available)
        ImGui::Text("ImGui Memory: %.2f KB", ImGui::GetIO().MetricsRenderVertices * sizeof(ImDrawVert) / 1024.0f);
        
        // Input information
        ImGui::Separator();
        ImGui::Text("Mouse Position: (%d, %d)", engine->mouseX, engine->mouseY);
        ImGui::Text("Mouse Buttons: %s", SDL_GetMouseState(nullptr, nullptr) ? "Pressed" : "Released");
        
        // SDL Information
        ImGui::Separator();
        ImGui::Text("SDL Version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
        
        // Renderer info
        // SDL_RendererInfo info;
        // if (SDL_GetRendererInfo(engine->getRenderer(), &info) == 0) {
        //     ImGui::Text("Renderer: %s", info.name);
        // }
    }
    ImGui::End();
}

void UIDebug::hideAllWindows() {
    showPerformanceWindow = false;
    showTileInspector = false;
    showLevelManager = false;
    showTilePalette = false;
    showCameraControls = false;
    showSystemInfo = false;
}

void UIDebug::showAllWindows() {
    showPerformanceWindow = true;
    showTileInspector = true;
    showLevelManager = true;
    showTilePalette = true;
    showCameraControls = true;
    showSystemInfo = true;
}