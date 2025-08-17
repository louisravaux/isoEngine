#include "Engine.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_mouse.h"

#include "UI/UIManager.hpp"
#include "core/TileRegistry.hpp"

IsoEngine::IsoEngine() {
    
}

IsoEngine::~IsoEngine() {
    
}

SDL_AppResult IsoEngine::EngineInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("IsoEngine", "0.0.1", "com.louisravaux.isoengine");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create a window and renderer
    if (!SDL_CreateWindowAndRenderer("IsoEngine", WIN_WIDTH, WIN_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // set window icon
    SDL_Surface* iconSurface = IMG_Load("assets/stone.png");
    if (iconSurface) {
        SDL_SetWindowIcon(window, iconSurface);
        SDL_DestroySurface(iconSurface);
    } else {
        SDL_Log("Failed to load icon.png: %s", SDL_GetError());
    }

    //Enable VSync
    if( SDL_SetRenderVSync( renderer, 1 ) == false )
    {
        SDL_Log( "Could not enable VSync! SDL error: %s\n", SDL_GetError() );
        return SDL_APP_FAILURE;
    }

    // hide mouse cursor
    SDL_HideCursor();

    // set window resizeable
    SDL_SetWindowResizable(window, true);

    // Load mouse cursor texture
    SDL_Surface* mouseCursorSurface = IMG_Load("assets/pencil.png");
    if (mouseCursorSurface) {
        mouseCursorTexture = SDL_CreateTextureFromSurface(renderer, mouseCursorSurface);
        SDL_DestroySurface(mouseCursorSurface);
        if (!mouseCursorTexture) {
            SDL_Log("Failed to create mouse cursor texture: %s", SDL_GetError());
        }
        SDL_SetTextureScaleMode(mouseCursorTexture, SDL_SCALEMODE_NEAREST);

    } else {
        SDL_Log("Failed to load mouse cursor.png: %s", SDL_GetError());
    }    

    // Load cursor texture
    SDL_Surface* cursorSurface = IMG_Load("assets/cursor.png");
    if (cursorSurface) {
        cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);
        SDL_DestroySurface(cursorSurface);
        if (!cursorTexture) {
            SDL_Log("Failed to create cursor texture: %s", SDL_GetError());
        }
        SDL_SetTextureScaleMode(cursorTexture, SDL_SCALEMODE_NEAREST);

    } else {
        SDL_Log("Failed to load cursor.png: %s", SDL_GetError());
    }

    // init gameLevels
    gameLevels.push_back(std::make_unique<Level>("Level 1"));
    gameLevels.push_back(std::make_unique<Level>("Level 2"));


    // Create a small test map
    int layerNumber = 2;
    gameLevels[0]->addMap(std::make_unique<Map>(8, 8, layerNumber, SDL_Color{ 135, 206, 235, 255 })); // Level 1, map 0
    gameLevels[0]->addMap(std::make_unique<Map>(12, 12, layerNumber, SDL_Color{ 65, 202, 165, 255 })); // Level 1, map 1
    gameLevels[0]->addMap(std::make_unique<Map>(50, 50, layerNumber, SDL_Color{ 114, 50, 50, 255 })); // Level 1, map 2

    // invert color to know it is level 2
    gameLevels[1]->addMap(std::make_unique<Map>(8, 8, layerNumber, SDL_Color{ 25, 206, 235, 255 })); // Level 2, map 0
    gameLevels[1]->addMap(std::make_unique<Map>(12, 12, layerNumber, SDL_Color{ 25, 202, 165, 255 })); // Level 2, map 1
    gameLevels[1]->addMap(std::make_unique<Map>(50, 50, layerNumber, SDL_Color{ 25, 50, 50, 255 })); // Level 2, map 2


    TileRegistry::registerType(0, "Void", renderer, "assets/void.png");
    TileRegistry::registerType(1, "Grass", renderer, "assets/grass.png");
    TileRegistry::registerType(2, "Sand", renderer, "assets/sand.png");
    TileRegistry::registerType(3, "Water", renderer, "assets/water.png");
    TileRegistry::registerType(4, "Stone", renderer, "assets/stone.png");
    TileRegistry::registerType(5, "Red Stone", renderer, "assets/red_stone.png");
    TileRegistry::registerType(6, "Lily pad", renderer, "assets/water_lily_pad.png");
    TileRegistry::registerType(7, "Mountains", renderer, "assets/mountains.png");

    // Fill the map with texture tiles
    // Create a simple checkerboard pattern
    for (int layer = 0; layer < 1; ++layer) {
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                if ((x + y) % 2 == 0) {
                    // Use grass texture for even positions
                    gameLevels[0]->getMap(0)->setTile(x, y, layer, 1);
                } else {
                    // Use sand texture for odd positions
                    gameLevels[0]->getMap(0)->setTile(x, y, layer, 2);
                }
            }
        }
    }

    for (int layer = 0; layer < 1; ++layer) {
        for (int y = 0; y < 12; ++y) {
            for (int x = 0; x < 12; ++x) {
                if ((x + y) % 2 == 0) {
                    gameLevels[0]->getMap(1)->setTile(x, y, layer, 2);
                } else {
                    gameLevels[0]->getMap(1)->setTile(x, y, layer, 3);
                }
            }
        }
    }

    for (int layer = 0; layer < 1; ++layer) {
        for (int y = 0; y < 50; ++y) {
            for (int x = 0; x < 50; ++x) {
                if ((x + y) % 2 == 0) {
                    gameLevels[0]->getMap(2)->setTile(x, y, layer, 4);
                } else {
                    gameLevels[0]->getMap(2)->setTile(x, y, layer, 5);
                }
            }
        }
    }

        for (int layer = 0; layer < 1; ++layer) {
        for (int y = 0; y < 50; ++y) {
            for (int x = 0; x < 50; ++x) {
                if ((x + y) % 2 == 0) {
                    gameLevels[1]->getMap(2)->setTile(x, y, layer, 4);
                } else {
                    gameLevels[1]->getMap(2)->setTile(x, y, layer, 5);
                }
            }
        }
    }
        
    // Add a special water tile in the center
    // gameMap->setTile(2, 2, 1, 3);
    // gameMap->setTile(2, 2, 2, 2);

    // Center the camera on the map
    gameLevels[0]->getMap(0)->setCamera(-WIN_WIDTH/2.0f, -WIN_HEIGHT/4.0f);
    gameLevels[0]->getMap(1)->setCamera(-WIN_WIDTH/2.0f, -WIN_HEIGHT/4.0f);
    gameLevels[0]->getMap(2)->setCamera(-WIN_WIDTH/2.0f, -WIN_HEIGHT/4.0f);

    gameLevels[1]->getMap(0)->setCamera(-WIN_WIDTH/2.0f, -WIN_HEIGHT/4.0f);
    gameLevels[1]->getMap(1)->setCamera(-WIN_WIDTH/2.0f, -WIN_HEIGHT/4.0f);
    gameLevels[1]->getMap(2)->setCamera(-WIN_WIDTH/2.0f, -WIN_HEIGHT/4.0f);

    uiManager = std::make_unique<UIDebug>(this);

    // Initialize UI Manager
    uiManager->init(window, renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult IsoEngine::EngineEvent(void *appstate, SDL_Event *event) 
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    // Handle UI events
    uiManager->event(event);
    ImGuiIO& io = uiManager->getIO();

    // Handle mouse movement for tile selection
    if (event->type == SDL_EVENT_MOUSE_MOTION && !io.WantCaptureMouse) {
        mouseX = static_cast<int>(event->motion.x);
        mouseY = static_cast<int>(event->motion.y);
        
        int gridX, gridY;
        if (gameLevels[activeLevelIndex]->getCurrentMap()->getSelectedTile(mouseX, mouseY, gridX, gridY)) {
            selectedTileX = gridX;
            selectedTileY = gridY;
        } else { // No valid tile selected
            selectedTileX = -1;
            selectedTileY = -1;
        }
    }

    // Handle mouse clicks
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && !io.WantCaptureMouse) {

        if (event->button.button == SDL_BUTTON_LEFT) {
            if (selectedTileX >= 0 && selectedTileY >= 0) {
                gameLevels[activeLevelIndex]->getCurrentMap()->setTile(selectedTileX, selectedTileY, selectedLayer, selectedTileType);
            }
        }
        if (event->button.button == SDL_BUTTON_MIDDLE) {
            uiManager->toggleVisibility();
        }
    }
    
    // Simple camera controls with arrow keys
    if (event->type == SDL_EVENT_KEY_DOWN && !io.WantCaptureKeyboard) {
        const float cameraSpeed = 32.0f;
        switch (event->key.key) {
            case SDLK_LEFT:
                gameLevels[activeLevelIndex]->getCurrentMap()->moveCamera(-cameraSpeed, 0);
                break;
            case SDLK_RIGHT:
                gameLevels[activeLevelIndex]->getCurrentMap()->moveCamera(cameraSpeed, 0);
                break;
            case SDLK_UP:
                gameLevels[activeLevelIndex]->getCurrentMap()->moveCamera(0, -cameraSpeed);
                break;
            case SDLK_DOWN:
                gameLevels[activeLevelIndex]->getCurrentMap()->moveCamera(0, cameraSpeed);
                break;
        }
    }

    // Reset camera zoom
    if (event->type == SDL_EVENT_MOUSE_WHEEL && !io.WantCaptureMouse) {
        if (event->wheel.y > 0) {
            gameLevels[activeLevelIndex]->getCurrentMap()->zoomCamera(1.05f);
        } else if (event->wheel.y < 0) {
            gameLevels[activeLevelIndex]->getCurrentMap()->zoomCamera(0.95f);
        }
    }

    if(event->type == SDL_EVENT_WINDOW_RESIZED) {
        // move camera to center
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        // center the camera on all maps
        for (auto& map : gameLevels[activeLevelIndex]->getAllMaps()) {
            map->setCamera(static_cast<float>(-windowWidth) / 2.0f, static_cast<float>(-windowHeight) / 4.0f);
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult IsoEngine::EngineIterate(void *appstate) 
{

    uiManager->update();

    // Clear screen to sky blue
    SDL_SetRenderDrawColor(renderer, gameLevels[activeLevelIndex]->getCurrentMap()->getBackgroundColor().r, gameLevels[activeLevelIndex]->getCurrentMap()->getBackgroundColor().g, gameLevels[activeLevelIndex]->getCurrentMap()->getBackgroundColor().b, gameLevels[activeLevelIndex]->getCurrentMap()->getBackgroundColor().a);
    SDL_RenderClear(renderer);

    // Render the map
    if (gameLevels[activeLevelIndex]->getCurrentMap()) {
        gameLevels[activeLevelIndex]->getCurrentMap()->renderWithCamera(renderer, gameLevels[activeLevelIndex]->getCurrentMap()->getCameraX(), gameLevels[activeLevelIndex]->getCurrentMap()->getCameraY());

        // Render cursor on selected tile
        if (cursorTexture && selectedTileX >= 0 && selectedTileY >= 0) {
            // Get the actual tile at this position
            Tile* selectedTile = gameLevels[activeLevelIndex]->getCurrentMap()->getTile(selectedTileX, selectedTileY, selectedLayer);
            if (selectedTile) {

                // Get zoom factor
                float zoom = gameLevels[activeLevelIndex]->getCurrentMap()->getCameraZoom();

                // Calculate cursor size with zoom
                const float CURSOR_SIZE = 64.0f * zoom;

                // Apply the same camera offset as the map does
                float cursorX = (selectedTile->getScreenX() * zoom) - CURSOR_SIZE * 0.5f - gameLevels[activeLevelIndex]->getCurrentMap()->getCameraX();
                float cursorY = (selectedTile->getScreenY() * zoom) - gameLevels[activeLevelIndex]->getCurrentMap()->getCameraY();

                SDL_FRect cursorRect = {
                    cursorX,
                    cursorY,
                    CURSOR_SIZE,
                    CURSOR_SIZE/2
                };
                
                SDL_RenderTexture(renderer, cursorTexture, nullptr, &cursorRect);
            }
        }
    }

    // draw mouse cursor
    SDL_FRect mouseCursorRect = {
        static_cast<float>(mouseX),
        static_cast<float>(mouseY) - 32.0f,
        32.0f, // Width of the cursor
        32.0f  // Height of the cursor
    };
    SDL_RenderTexture(renderer, mouseCursorTexture, nullptr, &mouseCursorRect);

    uiManager->content();
    uiManager->render(renderer);

    // Present the frame
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void IsoEngine::EngineQuit(void *appstate, SDL_AppResult result) 
{
    // Cleanup
    gameLevels[activeLevelIndex].reset(); // Destroy the maps

    if (cursorTexture) {
        SDL_DestroyTexture(cursorTexture);
        cursorTexture = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    uiManager->shutdown();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// getters

SDL_Window* IsoEngine::getWindow() const {
    return window;
}