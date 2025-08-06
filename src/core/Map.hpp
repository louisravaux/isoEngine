// Map.hpp
#pragma once

#include "Tile.hpp"
#include <vector>
#include <memory>

class Map {

private:
    int mapWidth, mapHeight;                                    // Dimensions of the map in tiles
    std::vector<std::vector<std::unique_ptr<Tile>>> tiles;      // 2D grid of tiles
    
    // Camera/viewport for scrolling
    float cameraX, cameraY;
    
    // Helper method to check if coordinates are valid
    bool isValidPosition(int x, int y) const;

public:
    // Constructor - creates empty map
    Map(int width, int height);
    
    // Destructor
    ~Map();
    
    // Tile management
    void setTile(int x, int y, std::unique_ptr<Tile> tile);
    void setTile(int x, int y, SDL_Renderer* renderer, const char* imagePath, int tileID);
    void removeTile(int x, int y);
    
    // Tile access
    Tile* getTile(int x, int y) const;
    bool hasTile(int x, int y) const;
    
    // Rendering
    void render(SDL_Renderer* renderer);
    void renderWithCamera(SDL_Renderer* renderer, float camX, float camY);
    
    // Camera control
    void setCamera(float x, float y);
    void moveCamera(float deltaX, float deltaY);
    float getCameraX() const;
    float getCameraY() const;
    
    // Map properties
    int getWidth() const;
    int getHeight() const;
    bool getSelectedTile(int screenX, int screenY, float cameraX, float cameraY, int& gridX, int& gridY) const;
    
    // Utility methods
    void clearMap();
    void fillWithTile(SDL_Renderer* renderer, const char* imagePath, int tileID);
    
    // Coordinate conversion helpers
    void screenToGrid(int screenX, int screenY, int& gridX, int& gridY) const;
    void gridToScreen(int gridX, int gridY, int& screenX, int& screenY) const;
};