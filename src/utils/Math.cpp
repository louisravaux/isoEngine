// Math.cpp

#include "Math.hpp"
#include <cmath>

void Math::toScreenCoordinates(int w, int h, int gridX, int gridY, int& screenX, int& screenY) {
    screenX = gridX * i_x * 0.5f * w + gridY * j_x * 0.5f * w;
    screenY = gridX * i_y * 0.5f * h + gridY * j_y * 0.5f * h;
}

void Math::toGridCoordinates(int w, int h, int screenX, int screenY, int& gridX, int& gridY) {    
    float a = i_x * 0.5f * w;
    float b = j_x * 0.5f * w;
    float c = i_y * 0.5f * h;
    float d = j_y * 0.5f * h;
        
    invertMatrix(a, b, c, d);
        
    float rawX = a * screenX + b * screenY;
    float rawY = c * screenX + d * screenY;
        
    gridX = static_cast<int>(std::floor(rawX));
    gridY = static_cast<int>(std::floor(rawY));
    
}

void Math::invertMatrix(float& a, float& b, float& c, float& d) {
    float det = (1.0f / (a * d - b * c));

    // Store original values
    float origA = a;
    float origD = d;

    a = det * origD;
    b = -det * b;
    c = -det * c;
    d = det * origA;
}
