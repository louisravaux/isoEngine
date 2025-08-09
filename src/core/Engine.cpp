#include "Engine.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_mouse.h"

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

    // Set vsync
    if (SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE)) {
        SDL_Log("Couldn't enable VSync: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // hide mouse cursor
    SDL_HideCursor();

    // set window resizeable
    SDL_SetWindowResizable(window, true);

    // Load mouse cursor texture
    SDL_Surface* mouseCursorSurface = IMG_Load("assets/mouse-cursor.png");
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

    // Create a small test map
    gameMap = std::make_unique<Map>(20, 20);

    // Fill the map with texture tiles
    // Create a simple checkerboard pattern
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 20; ++x) {
            if ((x + y) % 2 == 0) {
                // Use grass texture for even positions
                gameMap->setTile(x, y, renderer, "assets/dirt.png", 1);
            } else {
                // Use stone texture for odd positions
                gameMap->setTile(x, y, renderer, "assets/sand.png", 2);
            }
        }
    }
    
    // Add a special water tile in the center
    gameMap->setTile(2, 2, renderer, "assets/water.png", 3);

    // Center the camera on the map
    gameMap->setCamera(-WIN_WIDTH/2.0f, -16.0f);

    return SDL_APP_CONTINUE;
}

SDL_AppResult IsoEngine::EngineEvent(void *appstate, SDL_Event *event) 
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    
    // Handle mouse movement for tile selection
    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        mouseX = static_cast<int>(event->motion.x);
        mouseY = static_cast<int>(event->motion.y);
        
        int gridX, gridY;
        if (gameMap->getSelectedTile(mouseX, mouseY, gameMap->getCameraX(), gameMap->getCameraY(), gridX, gridY)) {
            selectedTileX = gridX;
            selectedTileY = gridY;
        } else { // No valid tile selected
            selectedTileX = -1;
            selectedTileY = -1;
        }
    }
    
    // Handle mouse clicks
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            if (selectedTileX >= 0 && selectedTileY >= 0) {
                SDL_Log("Clicked tile at (%d, %d)", selectedTileX, selectedTileY);
                SDL_Log("Screen position: (%d, %d)", mouseX, mouseY);
                SDL_Log("Camera position: (%f, %f)", gameMap->getCameraX(), gameMap->getCameraY());
                SDL_Log("Tile screen position: (%d, %d)", gameMap->getTile(selectedTileX, selectedTileY)->getScreenX(), gameMap->getTile(selectedTileX, selectedTileY)->getScreenY());

                // You could add tile interaction here, like changing tile type
                //gameMap->setTile(selectedTileX, selectedTileY, renderer, "assets/sand.png", 4);
            }
        }
    }
    
    // Simple camera controls with arrow keys
    if (event->type == SDL_EVENT_KEY_DOWN) {
        const float cameraSpeed = 32.0f;
        switch (event->key.key) {
            case SDLK_LEFT:
                gameMap->moveCamera(-cameraSpeed, 0);
                break;
            case SDLK_RIGHT:
                gameMap->moveCamera(cameraSpeed, 0);
                break;
            case SDLK_UP:
                gameMap->moveCamera(0, -cameraSpeed);
                break;
            case SDLK_DOWN:
                gameMap->moveCamera(0, cameraSpeed);
                break;
        }
    }
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult IsoEngine::EngineIterate(void *appstate) 
{
    // Clear screen to black
    SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
    SDL_RenderClear(renderer);

    // Render the map
    if (gameMap) {
        gameMap->renderWithCamera(renderer, gameMap->getCameraX(), gameMap->getCameraY());
        
        // Render cursor on selected tile
        if (cursorTexture && selectedTileX >= 0 && selectedTileY >= 0) {
            // Get the actual tile at this position
            Tile* selectedTile = gameMap->getTile(selectedTileX, selectedTileY);
            if (selectedTile) {
                // Use the exact same positioning as the tile itself
                int tileScreenX = selectedTile->getScreenX();
                int tileScreenY = selectedTile->getScreenY();

                const float CURSOR_SIZE = 64.0f;
                
                // Apply the same camera offset as the map does
                float cursorX = static_cast<float>(tileScreenX - CURSOR_SIZE * 0.5) - gameMap->getCameraX();
                float cursorY = static_cast<float>(tileScreenY) - gameMap->getCameraY();
                
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
        static_cast<float>(mouseX) - 16.0f,
        static_cast<float>(mouseY) - 16.0f,
        32.0f, // Width of the cursor
        32.0f  // Height of the cursor
    };
    SDL_RenderTexture(renderer, mouseCursorTexture, nullptr, &mouseCursorRect);


    // Present the frame
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void IsoEngine::EngineQuit(void *appstate, SDL_AppResult result) 
{
    // Cleanup
    gameMap.reset(); // Destroy the map
    
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
    
    SDL_Quit();
}