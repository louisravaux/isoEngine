// Engine.hpp

#pragma once

#include "SDL3/SDL_init.h"
#include <SDL3/SDL.h>

class IsoEngine {

private:
    const int WIN_WIDTH = 640;
    const int WIN_HEIGHT = 480;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

public:
    IsoEngine();
    ~IsoEngine();

    SDL_AppResult EngineInit(void **appstate, int argc, char *argv[]);
    SDL_AppResult EngineEvent(void *appstate, SDL_Event *event);
    SDL_AppResult EngineIterate(void *appstate);
    void EngineQuit(void *appstate, SDL_AppResult result);
};
