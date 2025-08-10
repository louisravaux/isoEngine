// Tile.cpp
#include "Tile.hpp"
#include <iostream>
#include "utils/Math.hpp"

// Isometric tile constants
constexpr int TILE_WIDTH = 64;   // Width of isometric tile sprite
constexpr int TILE_HEIGHT = 64;  // Height of isometric tile sprite

// Constructor - loads texture from file
Tile::Tile(int id, int posX, int posY)
    : tileID(id), gridX(posX), gridY(posY) {

    // Calculate screen position from grid position
    updateScreenPosition();
}

// Destructor
Tile::~Tile() {
    auto type = TileRegistry::getType(tileID);
    if (type) {
        // Texture will be freed if handled in TileType destructor
    }
}

// Render the tile
void Tile::render(SDL_Renderer* renderer) {
    auto type = TileRegistry::getType(tileID);
    if (!type) {
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
    SDL_RenderTexture(renderer, getTexture(), nullptr, &destRect);
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
    auto type = TileRegistry::getType(tileID);
    if (!type) return nullptr;
    return type->getTexture();
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