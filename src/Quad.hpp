#ifndef QUAD_H
#define QUAD_H

struct Quad {
    int index = -1;
    int patchIndex = -1;
    int neighbors[4] = {-1, -1, -1, -1};
    int edgeIndices[4] = {-1, -1, -1, -1};
    int x = 0;
    int y = 0;
};

#endif
