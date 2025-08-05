// Math.hpp

#pragma once

class Math {

private:
    constexpr static float i_x = 1.0f;
    constexpr static float i_y = 0.5f;
    constexpr static float j_x = -1.0f;
    constexpr static float j_y = 0.5f;

public:
    static void toScreenCoordinates(int w, int h, int gridX, int gridY, int& screenX, int& screenY);

    static void invertMatrix(float &a, float &b, float &c, float &d);
    static void toGridCoordinates(int w, int h, int screenX, int screenY, int& gridX, int& gridY);

};