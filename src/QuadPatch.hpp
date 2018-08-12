#ifndef QUADPATCH_H
#define QUADPATCH_H

#include <vector>

#include "Quad.hpp"

struct QuadPatch {
    std::vector<int> quadIndices;
    int width = 0;
    int height = 0;
    int index = 0;

    uint x = -1;
    uint y = -1;
};

#endif
