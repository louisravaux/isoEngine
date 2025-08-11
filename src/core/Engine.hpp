// Engine.hpp

#pragma once

#include "Map.hpp"
#include "SDL3/SDL_init.h"
#include <SDL3/SDL.h>
#include <memory>

#include "UI/UIDebug.hpp"

class IsoEngine {

private:
    const int WIN_WIDTH = 1280;
    const int WIN_HEIGHT = 720;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    std::unique_ptr<UIManager> uiManager = nullptr;

public:
    IsoEngine();
    ~IsoEngine();

    // Cursors
    SDL_Texture* cursorTexture = nullptr;
    SDL_Texture* mouseCursorTexture = nullptr;


    // selection
    int selectedTileX = -1;  // -1 means no tile selected
    int selectedTileY = -1;
    int mouseX = 0, mouseY = 0;
    int selectedTileType = 1;
    int selectedLayer = 0;

    // Game objects
    int activeMapIndex = 0;
    std::vector<std::unique_ptr<Map>> gameMaps;
    //std::unique_ptr<Map> gameMap;

    SDL_AppResult EngineInit(void **appstate, int argc, char *argv[]);
    SDL_AppResult EngineEvent(void *appstate, SDL_Event *event);
    SDL_AppResult EngineIterate(void *appstate);
    void EngineQuit(void *appstate, SDL_AppResult result);
    
};
