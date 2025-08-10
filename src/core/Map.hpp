// Map.hpp
#pragma once

#include "Tile.hpp"
#include <vector>
#include <memory>

class Map {

private:
    int mapWidth, mapHeight, numLayers;                                    // Dimensions of the map in tiles
    std::vector<std::vector<std::vector<std::unique_ptr<Tile>>>> tiles;      // 3D grid of tiles
    
    // Camera/viewport for scrolling
    float cameraX, cameraY;
    
    // Helper method to check if coordinates are valid
    bool isValidPosition(int x, int y) const;

public:
    // Constructor - creates empty map
    Map(int width, int height, int numLayers);
    
    // Destructor
    ~Map();
    
    // Tile management
    void setTile(int x, int y, int layer, std::unique_ptr<Tile> tile);
    void setTile(int x, int y, int layer, int tileID);
    void removeTile(int x, int y, int layer);
    
    // Tile access
    Tile* getTile(int x, int y, int layer) const;
    bool hasTile(int x, int y, int layer) const;

    // Rendering
    //void render(SDL_Renderer* renderer, int layer);
    void renderWithCamera(SDL_Renderer* renderer, float camX, float camY);
    
    // Camera control
    void setCamera(float x, float y);
    void moveCamera(float deltaX, float deltaY);
    float getCameraX() const;
    float getCameraY() const;
    
    // Map properties
    int getWidth() const;
    int getHeight() const;
    int getLayerCount() const;
    bool getSelectedTile(int screenX, int screenY, float cameraX, float cameraY, int& gridX, int& gridY) const;
    
    // Utility methods
    void clearMap();
    void fillWithTile(int tileID, int layer);
    
    // Coordinate conversion helpers
    void screenToGrid(int screenX, int screenY, int& gridX, int& gridY) const;
    void gridToScreen(int gridX, int gridY, int& screenX, int& screenY) const;
};