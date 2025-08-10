#include "TileRegistry.hpp"
#include <iostream>
#include <SDL3_image/SDL_image.h>

std::unordered_map<int, std::shared_ptr<TileType>> TileRegistry::registry;

void TileRegistry::registerType(int id, const std::string& name, SDL_Renderer* renderer, const char* imagePath) {

    SDL_Texture* texture = nullptr;

    texture = loadSprite(renderer, imagePath);

    registry[id] = std::make_shared<TileType>(name, texture);
}

SDL_Texture* TileRegistry::loadSprite(SDL_Renderer* renderer, const char* imagePath) {
    SDL_Surface* surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_Log("Failed to load image %s: %s", imagePath, SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create texture from %s: %s", imagePath, SDL_GetError());
    } else {
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    }

    SDL_DestroySurface(surface);
    return texture;
}

std::shared_ptr<TileType> TileRegistry::getType(int id) {
    auto it = registry.find(id);
    return (it != registry.end()) ? it->second : nullptr;
}

int TileRegistry::getTileID(const TileType* tile) {
    for (const auto& pair : registry) {
        if (pair.second.get() == tile) {
            return pair.first;
        }
    }
    return -1;  // Not found
}

std::vector<const TileType*> TileRegistry::getAllTypes() {
    std::vector<const TileType*> types;
    for (const auto& pair : registry) {
        types.push_back(pair.second.get());
    }
    return types;
}


void TileRegistry::clear() {
    registry.clear(); 
}