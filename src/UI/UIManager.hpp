#pragma once

#include <SDL3/SDL.h>

class  UIManager {

    private:
        bool isHidden = false;

    public:
        UIManager();
        ~UIManager();

        void init(SDL_Window *window, SDL_Renderer *renderer);
        virtual void content();
        void event(SDL_Event *event);
        void update();
        void render(SDL_Renderer *renderer);
        void shutdown();
};