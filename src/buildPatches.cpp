#include "buildPatches.hpp"

// Gets the index of the neighbor "across" the quad from a given neighbor
int crossQuad(int neighborIndex, Quad quad) {
    int crossIndex = 2;

    crossIndex = quad.neighbors[1] == neighborIndex ? 3 : crossIndex;
    crossIndex = quad.neighbors[2] == neighborIndex ? 0 : crossIndex;
    crossIndex = quad.neighbors[3] == neighborIndex ? 1 : crossIndex;

    return crossIndex;
}

void growPatch(Quad &quad, std::vector<Quad> &quads, QuadPatch& quadPatch, int dimensionLimit) {
    int width = 0;
    int height = 0;
    Quad currentQuad = quad;
    for (int x = 1; x < dimensionLimit; x++) {
        for (int y = 0; y < dimensionLimit; y++) {
            
            // Travel in one direction

            // Travel in the opposite direction

        }
        // Travel over
        
    }
}

void buildPatches(std::vector<Quad> &quads, std::vector<QuadPatch> &quadPatches, int dimensionLimit) {
    int currentPatchIndex = 0;
    int quadCount = quads.size();
    for (int i = 0; i < quadCount; i++) {
        Quad quad = quads[i];
        if (quad.patchIndex == -1) {
            QuadPatch quadPatch;
            quadPatches.push_back(quadPatch);
            quad.patchIndex = currentPatchIndex;
            growPatch(quad, quads, quadPatch, dimensionLimit);
            currentPatchIndex++;
        }
    }
}
