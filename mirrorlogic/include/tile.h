#pragma once

#include <iostream>
#include <cstdint>
#include <cmath>

using std::int8_t;

class Tile {
    public:
        Tile();
        Tile(Tile* parent, int8_t parentMountIndex, float flatWidth);
        int8_t ComputeTileLocationRelativeToParent();
    private:
        Tile* parent;
        Tile* child;
        int8_t parentMountIndex;

        // measurements in cm
        float xFromParent;
        float yFromParent;
        float xFromBase;
        float yFromBase; 
        float flatWidth;
};