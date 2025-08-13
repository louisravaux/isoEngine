#pragma once

#include <SDL3/SDL.h>
#include <imgui.h>

class  UIManager {
    private:
        // private stuff

    protected:
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

        //getters
        ImGuiIO& getIO();
        bool getIsHidden();

        //setters
        void toggleVisibility();
};