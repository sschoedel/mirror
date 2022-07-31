#pragma once

#include <tile.h>

#include <fstream>
#include <cstdint>
#include <algorithm>

using std::uint8_t;

class MirrorGrid {
    public:
        MirrorGrid(char* cfgFileName);
    private:
        uint8_t ParseConfigFile(char* cfgFileName);
        Tile* head;
        int num_tiles;
        float base_tile_location_x;
        float base_tile_location_y;
        bool simulating;
};