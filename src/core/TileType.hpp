#pragma once

#include <string>
#include <SDL3/SDL.h>

class TileType {
private:
    std::string name;
    SDL_Texture* texture;

public:
    TileType(const std::string& name, SDL_Texture* texture);
    ~TileType();

    const std::string& getName() const;
    SDL_Texture* getTexture() const;


};
