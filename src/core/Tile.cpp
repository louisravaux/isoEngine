// Tile.cpp
#include "Tile.hpp"
#include <iostream>
#include "utils/Math.hpp"


// Constructor - loads texture from file
Tile::Tile(int id, int posX, int posY, int width, int height)
    : tileID(id), gridX(posX), gridY(posY), width(width), height(height) {

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

// Getters
int Tile::getID() const {
    return tileID;
}

int Tile::getWidth() const {
    return width;
}

int Tile::getHeight() const {
    return height;
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
    Math::toScreenCoordinates(width, height, gridX, gridY, screenX, screenY);
}