// Tile.hpp

#pragma once

#include <string>

#include <SDL3_image/SDL_image.h>
#include "TileRegistry.hpp"

class Tile {

private: 
    int tileID; // ID of the tile type
    int gridX, gridY;    // Position in tile grid (0,0), (1,0), etc.
    int screenX, screenY; // Actual screen position in pixels

public:
    // Constructor to initialize the tile with a texture, ID, and position
    Tile(int id, int posX, int posY);

    // Destructor to clean up the texture
    ~Tile();

    // Update screen position based on grid position
    void updateScreenPosition();

    // Getters for tile properties
    int getID() const;
    int getGridX() const;
    int getGridY() const;
    int getScreenX() const;
    int getScreenY() const;
    SDL_Texture* getTexture() const;

    // Setters for tile properties
    void setPosition(int posX, int posY);
    void setID(int id);
};