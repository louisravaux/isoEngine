// Map.cpp
#include "Map.hpp"
#include <iostream>

// Tile constants (should match Tile.cpp)
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;

// Constructor - creates empty map
Map::Map(int width, int height) 
    : mapWidth(width), mapHeight(height), cameraX(0.0f), cameraY(0.0f) {
    
    // Initialize 2D vector with empty tiles
    tiles.resize(mapHeight);
    for (int y = 0; y < mapHeight; ++y) {
        tiles[y].resize(mapWidth);
        // unique_ptr automatically initializes to nullptr
    }
}

// Destructor
Map::~Map() {
    // unique_ptr automatically cleans up, nothing needed here
}

// Helper method to check if coordinates are valid
bool Map::isValidPosition(int x, int y) const {
    return (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight);
}

// Tile management - set tile with existing unique_ptr
void Map::setTile(int x, int y, std::unique_ptr<Tile> tile) {
    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid tile position: (" << x << ", " << y << ")" << std::endl;
        return;
    }
    
    // Move the unique_ptr (old tile automatically destroyed if exists)
    tiles[y][x] = std::move(tile);
}

// Tile management - create new tile from image
void Map::setTile(int x, int y, SDL_Renderer* renderer, const char* imagePath, int tileID) {
    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid tile position: (" << x << ", " << y << ")" << std::endl;
        return;
    }
    
    // Create new tile and move it into position
    auto tile = std::make_unique<Tile>(renderer, imagePath, tileID, x, y);
    tiles[y][x] = std::move(tile);
}

// Remove tile at position
void Map::removeTile(int x, int y) {
    if (!isValidPosition(x, y)) {
        return;
    }
    
    // Reset unique_ptr (automatically destroys the tile)
    tiles[y][x].reset();
}

// Get tile at position
Tile* Map::getTile(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return nullptr;
    }
    
    return tiles[y][x].get(); // get() returns raw pointer from unique_ptr
}

// Check if tile exists at position
bool Map::hasTile(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    
    return tiles[y][x] != nullptr;
}

// Render all tiles
void Map::render(SDL_Renderer* renderer) {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (tiles[y][x]) {
                tiles[y][x]->render(renderer);
            }
        }
    }
}

// Render with camera offset
void Map::renderWithCamera(SDL_Renderer* renderer, float camX, float camY) {
    // Store current camera position
    float oldCamX = cameraX;
    float oldCamY = cameraY;
    
    // Set new camera position
    cameraX = camX;
    cameraY = camY;
    
    // Render tiles with offset
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (tiles[y][x]) {
                // Get tile's screen position
                int tileScreenX = tiles[y][x]->getScreenX();
                int tileScreenY = tiles[y][x]->getScreenY();
                
                // Apply camera offset
                SDL_FRect destRect = {
                    static_cast<float>(tileScreenX) - cameraX,
                    static_cast<float>(tileScreenY) - cameraY,
                    static_cast<float>(TILE_WIDTH),
                    static_cast<float>(TILE_HEIGHT)
                };
                
                // Render tile at offset position
                SDL_RenderTexture(renderer, tiles[y][x]->getTexture(), nullptr, &destRect);
            }
        }
    }
    
    // Restore camera position
    cameraX = oldCamX;
    cameraY = oldCamY;
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

// Clear all tiles
void Map::clearMap() {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            tiles[y][x].reset(); // Destroy tile and set to nullptr
        }
    }
}

// Fill entire map with same tile type
void Map::fillWithTile(SDL_Renderer* renderer, const char* imagePath, int tileID) {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            setTile(x, y, renderer, imagePath, tileID);
        }
    }
}

// Convert screen coordinates to grid coordinates
void Map::screenToGrid(int screenX, int screenY, int& gridX, int& gridY) const {
    // Apply camera offset first
    float adjustedX = screenX + cameraX;
    float adjustedY = screenY + cameraY;
    
    // Inverse isometric transformation
    // Given: screenX = (gridX - gridY) * (TILE_WIDTH / 2)
    //        screenY = (gridX + gridY) * (TILE_HEIGHT / 2)
    // Solve for gridX, gridY
    
    float tileX = adjustedX / (TILE_WIDTH / 2.0f);
    float tileY = adjustedY / (TILE_HEIGHT / 2.0f);
    
    gridX = static_cast<int>((tileX + tileY) / 2.0f);
    gridY = static_cast<int>((tileY - tileX) / 2.0f);
}

// Convert grid coordinates to screen coordinates
void Map::gridToScreen(int gridX, int gridY, int& screenX, int& screenY) const {
    // Standard isometric transformation
    screenX = (gridX - gridY) * (TILE_WIDTH / 2);
    screenY = (gridX + gridY) * (TILE_HEIGHT / 2);
}


bool Map::isPointInDiamond(float pointX, float pointY, int tileX, int tileY) const {
    
    // Translate point to tile-local coordinates
    float localX = pointX - tileX;
    float localY = pointY - tileY;
    
    // Diamond hit detection - check if point is within diamond bounds
    float centerX = TILE_WIDTH / 2.0f;
    float centerY = TILE_HEIGHT / 4.0f;
    
    // Diamond equation: |x - centerX|/halfWidth + |y - centerY|/halfHeight <= 1
    float normalizedX = abs(localX - centerX) / centerX;
    float normalizedY = abs(localY - centerY) / centerY;
    
    return (normalizedX + normalizedY) <= 1.0f;
}

bool Map::getSelectedTile(int screenX, int screenY, float cameraX, float cameraY, int& gridX, int& gridY) const {
    // Adjust for camera
    float adjustedX = screenX + cameraX;
    float adjustedY = screenY + cameraY;
    
    // First, get approximate grid position using standard conversion
    float tileX = adjustedX / (TILE_WIDTH/2.0f);  // Half tile width
    float tileY = adjustedY / (TILE_HEIGHT/4.0f);   // Quarter tile height
    
    int approxGridX = static_cast<int>((tileX + tileY) / 2.0f);
    int approxGridY = static_cast<int>((tileY - tileX) / 2.0f);
    
    // Check a 3x3 area around the approximate position for more precision
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int testGridX = approxGridX + dx;
            int testGridY = approxGridY + dy;
            
            // Skip if out of bounds
            if (testGridX < 0 || testGridX >= mapWidth || testGridY < 0 || testGridY >= mapHeight) {
                continue;
            }
            
            // Convert back to screen coordinates for this tile
            int tileScreenX = (testGridX - testGridY) * (TILE_WIDTH/2);
            int tileScreenY = (testGridX + testGridY) * (TILE_HEIGHT/4);
            
            // Check if point is within diamond shape of this tile
            if (isPointInDiamond(adjustedX, adjustedY, tileScreenX, tileScreenY)) {
                gridX = testGridX;
                gridY = testGridY;
                return true;
            }
        }
    }
    
    return false; // No tile selected
}

