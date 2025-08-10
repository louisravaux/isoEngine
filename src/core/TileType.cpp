#include "TileType.hpp"

TileType::TileType(const std::string& name, SDL_Texture* texture)
    : name(name), texture(texture) {}

TileType::~TileType() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

const std::string& TileType::getName() const {
    return name;
}

SDL_Texture* TileType::getTexture() const {
    return texture;
}
