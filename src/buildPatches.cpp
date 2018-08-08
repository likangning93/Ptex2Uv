#include "buildPatches.hpp"

// Gets the index of the neighbor "across" the quad from a given neighbor
int crossQuad(int neighborIndex, Quad &quad) {
    int crossIndex = 1;

    crossIndex = quad.neighbors[0] == neighborIndex ? 2 : crossIndex;
    crossIndex = quad.neighbors[1] == neighborIndex ? 3 : crossIndex;
    crossIndex = quad.neighbors[2] == neighborIndex ? 0 : crossIndex;

    return quad.neighbors[crossIndex];
}

// Gets the index of the neighbor "across" the quad from a given neighbor
int ccwiseNeighbor(int neighborIndex, Quad &quad) {
    int ccwiseIndex = 0;

    ccwiseIndex = quad.neighbors[0] == neighborIndex ? 1 : ccwiseIndex;
    ccwiseIndex = quad.neighbors[1] == neighborIndex ? 2 : ccwiseIndex;
    ccwiseIndex = quad.neighbors[2] == neighborIndex ? 3 : ccwiseIndex;

    return quad.neighbors[ccwiseIndex];
}

bool nextQuadNotOk(int previousIndex, int currentPatchIndex, Quad &quad, std::vector<Quad> &quads) {
    Quad nextQuad = quads[crossQuad(previousIndex, quad)];
    // If quad belongs to a patch, not OK.
    // If nextQuad belongs to the same patch, not OK.
    return quad.patchIndex != -1 || currentPatchIndex == nextQuad.patchIndex;
}

void growPatch(Quad* quad, std::vector<Quad> &quads, QuadPatch &quadPatch, int dimensionLimit) {
    int width = 0;
    int height = 0;
    Quad* currentQuad = quad;
    Quad* nextXquad = &quads[quad->neighbors[0]];
    Quad* nextYquad = &quads[ccwiseNeighbor(nextXquad->index, *currentQuad)];
    
    for (int x = 0; x < dimensionLimit; x++) {
        int prevXquadIndex = currentQuad->index;

        for (int y = 0; y < dimensionLimit; y++) {
            currentQuad->x = x;
            currentQuad->y = y;

            // Add this quad to the patch
            currentQuad->patchIndex = quadPatch.index;
            quadPatch.quadIndices.push_back(currentQuad->index);
            quadPatch.height = std::max(quadPatch.height, y);

            int prevYquadIndex = currentQuad->index;
            currentQuad = nextYquad;

            // check if new currentQuad OK or if should break
            if (nextQuadNotOk(prevYquadIndex, quadPatch.index, *currentQuad, quads)) {
                break;
            }
            nextYquad = &quads[crossQuad(prevYquadIndex, *currentQuad)];
        }
        quadPatch.width = std::max(quadPatch.width, x);

        currentQuad = nextXquad;

        // check if new currentQuad OK or if should break
        if (nextQuadNotOk(prevXquadIndex, quadPatch.index, *currentQuad, quads)) {
            return;
        }
        nextXquad = &quads[crossQuad(prevXquadIndex, *currentQuad)];
        nextYquad = &quads[ccwiseNeighbor(nextXquad->index, *currentQuad)];
    }
}

void buildPatches(std::vector<Quad> &quads, std::vector<QuadPatch> &quadPatches, int dimensionLimit) {
    int currentPatchIndex = 1;
    int quadCount = quads.size();
    for (int i = 0; i < quadCount; i++) {
        Quad* quad = &quads[i];
        if (quad->patchIndex == -1) {
            QuadPatch quadPatch;
            quadPatch.index = currentPatchIndex++;
            growPatch(quad, quads, quadPatch, dimensionLimit);
            quadPatch.height++;
            quadPatch.width++;
            quadPatches.push_back(quadPatch);
        }
    }
}
