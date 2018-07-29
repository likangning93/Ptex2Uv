#ifndef QUAD_H
#define QUAD_H

struct Quad {
    int patchIndex = -1;
    int neighbors[4] = {-1, -1, -1, -1};
    int edgeIndices[4] = {-1, -1, -1, -1};
};

#endif
