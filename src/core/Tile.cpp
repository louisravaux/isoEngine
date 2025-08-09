// Tile.cpp
#include "Tile.hpp"
#include <iostream>
#include "utils/Math.hpp"

// Isometric tile constants
constexpr int TILE_WIDTH = 64;   // Width of isometric tile sprite
constexpr int TILE_HEIGHT = 64;  // Height of isometric tile sprite

// Constructor - loads texture from file
Tile::Tile(SDL_Renderer* renderer, const char* imagePath, int id, int posX, int posY)
    : tileID(id), gridX(posX), gridY(posY) {
    
    // Load image as surface first
    SDL_Surface* surface = IMG_Load(imagePath);
    if (!surface) {
        std::cerr << "Failed to load tile image " << imagePath << ": " << SDL_GetError() << std::endl;
        sprite = nullptr;
        return;
    }
    
    // Convert surface to texture
    sprite = SDL_CreateTextureFromSurface(renderer, surface);
    if (!sprite) {
        std::cerr << "Failed to create texture from " << imagePath << ": " << SDL_GetError() << std::endl;
    } else {
        SDL_SetTextureScaleMode(sprite, SDL_SCALEMODE_NEAREST);
    }
    
    // Clean up surface (no longer needed)
    SDL_DestroySurface(surface);
    
    // Calculate screen position from grid position
    updateScreenPosition();
}

// Constructor - uses existing texture
Tile::Tile(SDL_Texture* texture, int id, int posX, int posY)
    : sprite(texture), tileID(id), gridX(posX), gridY(posY) {

    screenX = 0;
    screenY = 0;
    
    // Calculate screen position from grid position
    updateScreenPosition();
}

// Destructor
Tile::~Tile() {
    if (sprite) {
        SDL_DestroyTexture(sprite);
        sprite = nullptr;
    }
}

// Render the tile
void Tile::render(SDL_Renderer* renderer) {
    if (!sprite) {
        return; // Nothing to render
    }
    
    // Create destination rectangle for rendering
    SDL_FRect destRect = {
        static_cast<float>(screenX),
        static_cast<float>(screenY),
        static_cast<float>(TILE_WIDTH),
        static_cast<float>(TILE_HEIGHT)
    };
    
    // Render the texture
    SDL_RenderTexture(renderer, sprite, nullptr, &destRect);
}

// Getters
int Tile::getID() const {
    return tileID;
}

int Tile::getGridX() const {
    return gridX;
}

int Tile::getGridY() const {
    return gridY;
}

int Tile::getScreenX() const {
    return screenX;
}

int Tile::getScreenY() const {
    return screenY;
}

SDL_Texture* Tile::getTexture() const {
    return sprite;
}

// Setters
void Tile::setPosition(int posX, int posY) {
    gridX = posX;
    gridY = posY;
    updateScreenPosition(); // Recalculate screen position
}

void Tile::setID(int id) {
    tileID = id;
}

void Tile::updateScreenPosition() {
    Math::toScreenCoordinates(TILE_WIDTH, TILE_HEIGHT, gridX, gridY, screenX, screenY);
}