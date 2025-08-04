#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "core/Engine.hpp"

IsoEngine engine;

int SDL_AppMain(int argc, char *argv[]) {
    void* appstate = nullptr;

    if (engine.EngineInit(&appstate, argc, argv) != SDL_APP_CONTINUE)
        return 1;

    SDL_Event e;
    SDL_AppResult result = SDL_APP_CONTINUE;

    while (result == SDL_APP_CONTINUE) {
        while (SDL_PollEvent(&e)) {
            result = engine.EngineEvent(appstate, &e);
            if (result != SDL_APP_CONTINUE)
                break;
        }
        if (result == SDL_APP_CONTINUE)
            result = engine.EngineIterate(appstate);
    }

    engine.EngineQuit(appstate, result);
    return result == SDL_APP_SUCCESS ? 0 : 1;
}

int main(int argc, char *argv[]) {
    return SDL_RunApp(argc, argv, SDL_AppMain, nullptr);
}