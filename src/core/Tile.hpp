// Tile.hpp

#pragma once

#include "SDL3/SDL_render.h"
#include <SDL3_image/SDL_image.h>

class Tile {

private: 
    SDL_Texture* sprite;
    int tileID; // type identifier
    int gridX, gridY;    // Position in tile grid (0,0), (1,0), etc.
    int screenX, screenY; // Actual screen position in pixels

public:

    // Constructor to initialize the tile with a texture, ID, and position
    Tile(SDL_Renderer* renderer, const char* imagePath, int id, int posX, int posY);

    // Constructor to initialize the tile with an existing texture
    Tile(SDL_Texture* texture, int id, int posX, int posY);

    // Destructor to clean up the texture
    ~Tile();

    // Render the tile at its position
    void render(SDL_Renderer* renderer);

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