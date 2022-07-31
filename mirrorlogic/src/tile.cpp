#include <tile.h>

Tile::Tile() {
    this->parent = nullptr;
    this->child == nullptr;
    this->parentMountIndex = -1;
    this->flatWidth = 3;
    xFromParent = 0;
    yFromParent = 0;
    xFromBase = 0;
    yFromBase = 0;
}

Tile::Tile(Tile* parent, int8_t parentMountIndex, float flatWidth) {
    this->parent = parent;
    this->child = nullptr;
    this->parentMountIndex = parentMountIndex;
    this->flatWidth = flatWidth;
    int8_t success = ComputeTileLocationRelativeToParent();
}

// Uses the index of the side of the parent touching its child (the current tile) to compute
// location relative to parent tile.
// Indices begin at the top horizontal edge of the hexagon (index 0) and continue clockwise.
// In the following example, tile 0 is the parent of tile 1 and tile 1's parentMountIndex is 2.
//
//            0
//        /¯¯ ¯¯ ¯¯\
//     5 /          \ 1
//      /    Tile    \
//      \     0      /¯¯ ¯¯ ¯¯\
//     4 \          / 2        \
//        \__ __ __/     Tile   \
//             3   \      1     /
//                  \          /
//                   \__ __ __/
//
int8_t Tile::ComputeTileLocationRelativeToParent() {
    // Ensure parent exists before doing computation
    if (this->parentMountIndex == -1) {
        std::cerr << "Tile does not have a parentMountIndex, can not compute relative location" << std::endl;
        return 0;
    }
    
    // Index 0 = -pi/2 rads
    // Every additional index adds pi/3 rads
    // x to the right, y down
    float angle = -M_PI/2 + this->parentMountIndex * M_PI/3;
    xFromParent = std::cos(angle) * this->flatWidth * 2;
    yFromParent = std::sin(angle) * this->flatWidth * 2;
    if (std::isfinite(xFromParent) && std::isfinite(yFromParent)) {
        return 1;
    }
    else {
        std::cerr << "Location relative to parent tile is not a finite value" << std::endl;
        return 0;
    }
}