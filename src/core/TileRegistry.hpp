#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "TileType.hpp"

class TileRegistry {
private:
    static std::unordered_map<int, std::shared_ptr<TileType>> registry;
    static SDL_Texture* loadSprite(SDL_Renderer* renderer, const char* imagePath);

public:
    static void registerType(int id, const std::string& name, SDL_Renderer* renderer, const char* imagePath);
    static std::shared_ptr<TileType> getType(int id);
    static int getTileID(const TileType* tile);
    static std::vector<const TileType*> getAllTypes();
    static void clear();
};