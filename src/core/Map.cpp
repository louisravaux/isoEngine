// Map.cpp
#include "Map.hpp"
#include "utils/Math.hpp"
#include <iostream>
#include <algorithm>

// Constructor - creates empty map
Map::Map(int width, int height, int numLayers, SDL_Color bgColor)
    : mapWidth(width), mapHeight(height), numLayers(numLayers), cameraX(0.0f), cameraY(0.0f), backgroundColor(bgColor) {

    // Set default tile size
    tileWidth = 64;
    tileHeight = 64;

    // Initialize 3D vector with empty tiles
    tiles.resize(numLayers);
    for (int z = 0; z < numLayers; ++z) {
        tiles[z].resize(mapHeight);
        for (int y = 0; y < mapHeight; ++y) {
            tiles[z][y].resize(mapWidth);
            // unique_ptr automatically initializes to nullptr
        }
    }
}

// Destructor
Map::~Map() {
    
}

// Helper method to check if coordinates are valid
bool Map::isValidPosition(int x, int y) const {
    return (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight);
}

// Tile management - set tile with existing unique_ptr
void Map::setTile(int x, int y, int layer, std::unique_ptr<Tile> tile) {
    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid tile position: (" << x << ", " << y << ")" << std::endl;
        return;
    }
    
    // Move the unique_ptr
    tiles[layer][y][x] = std::move(tile);
}

// Tile management - create new tile from image
void Map::setTile(int x, int y, int layer, int tileID) {
    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid tile position: (" << x << ", " << y << ")" << std::endl;
        return;
    }
    
    // Create new tile and move it into position
    auto tile = std::make_unique<Tile>(tileID, x, y, tileWidth, tileHeight);
    tiles[layer][y][x] = std::move(tile);
}

// Remove tile at position
void Map::removeTile(int x, int y, int layer) {
    if (!isValidPosition(x, y)) {
        return;
    }
    
    // Reset unique_ptr (automatically destroys the tile)
    tiles[layer][y][x].reset();
}

// Get tile at position
Tile* Map::getTile(int x, int y, int layer) const {
    if (!isValidPosition(x, y)) {
        return nullptr;
    }

    return tiles[layer][y][x].get(); // get() returns raw pointer from unique_ptr
}

// Check if tile exists at position
bool Map::hasTile(int x, int y, int layer) const {
    if (!isValidPosition(x, y)) {
        return false;
    }

    return tiles[layer][y][x] != nullptr;
}

// Render with camera offset
void Map::renderWithCamera(SDL_Renderer* renderer, float camX, float camY) {
    // Store current camera position
    float oldCamX = cameraX;
    float oldCamY = cameraY;
    
    // Set new camera position
    cameraX = camX;
    cameraY = camY;
    
    // Calculate zoomed tile dimensions for rendering only
    float zoomedTileWidth = tileWidth * cameraZoom;
    float zoomedTileHeight = tileHeight * cameraZoom;
    
    // Render tiles with layer as outermost loop for proper layering
    for (int layer = 0; layer < numLayers; ++layer) {
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                if (tiles[layer][y][x]) {
                    // Get tile's base screen position
                    int tileScreenX = tiles[layer][y][x]->getScreenX();
                    int tileScreenY = tiles[layer][y][x]->getScreenY();
                    
                    // Apply zoom to the position
                    float zoomedX = tileScreenX * cameraZoom;
                    float zoomedY = tileScreenY * cameraZoom;

                    // Apply camera offset
                    SDL_FRect destRect = {
                        zoomedX - zoomedTileWidth * 0.5f - cameraX,
                        zoomedY - cameraY - layer * zoomedTileHeight * 0.5f,
                        zoomedTileWidth,
                        zoomedTileHeight
                    };
                    
                    // Render tile at offset position
                    SDL_RenderTexture(renderer, tiles[layer][y][x]->getTexture(), nullptr, &destRect);
                }
            }
        }
    }
}

// Camera control
void Map::setCamera(float x, float y) {
    cameraX = x;
    cameraY = y;
}

void Map::moveCamera(float deltaX, float deltaY) {
    cameraX += deltaX;
    cameraY += deltaY;
}

void Map::zoomCamera(float zoomFactor) {
    cameraZoom *= zoomFactor;
    // Ensure zoom factor is within reasonable limits
    cameraZoom = std::clamp(cameraZoom, 0.5f, 4.0f);
}

float Map::getCameraZoom() const {
    return cameraZoom;
}

float Map::getCameraX() const {
    return cameraX;
}

float Map::getCameraY() const {
    return cameraY;
}

// Map properties
int Map::getWidth() const {
    return mapWidth;
}

int Map::getHeight() const {
    return mapHeight;
}

int Map::getLayerCount() const {
    return numLayers;
}

// Clear all tiles
void Map::clearMap() {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            tiles[numLayers - 1][y][x].reset(); // Destroy tile and set to nullptr
        }
    }
}

// Fill entire map with same tile type
void Map::fillWithTile(int tileID, int layer) {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            setTile(x, y, layer, tileID);
        }
    }
}

// Convert screen coordinates to grid coordinates
void Map::screenToGrid(int screenX, int screenY, int& gridX, int& gridY) const {
    Math::toGridCoordinates(tileWidth, tileHeight, screenX, screenY, gridX, gridY);
}

// Convert grid coordinates to screen coordinates
void Map::gridToScreen(int gridX, int gridY, int& screenX, int& screenY) const {
    Math::toScreenCoordinates(tileWidth, tileHeight, gridX, gridY, screenX, screenY);
}

bool Map::getSelectedTile(int screenX, int screenY, int& gridX, int& gridY) const {

    float adjustedX = (screenX + getCameraX()) / cameraZoom;
    float adjustedY = (screenY + getCameraY()) / cameraZoom;
    
    screenToGrid(adjustedX, adjustedY, gridX, gridY);

    // Check if resulting grid coordinates are valid
    if (gridX < 0 || gridY < 0 || gridX >= mapWidth || gridY >= mapHeight) {
        return false;
    }
    
    return true;
}

SDL_Color Map::getBackgroundColor() const {
    return backgroundColor;
}

void Map::setBackgroundColor(const SDL_Color& color) {
    backgroundColor = color;
}
