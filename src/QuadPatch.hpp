#ifndef QUADPATCH_H
#define QUADPATCH_H

#include <vector>

#include "Quad.hpp"

struct QuadPatch {
    std::vector<Quad> quads;
    int width = 0;
    int height = 0;
};

#endif
