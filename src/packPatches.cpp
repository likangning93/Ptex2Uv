#include "packPatches.hpp"

bool rasterOk(int x, int y, QuadPatch &quadPatch, PackingGrid &grid, std::vector<Quad> &quads)
{
    int quadCount = quadPatch.quadIndices.size();
    if (quadPatch.width + x >= grid.width || quadPatch.height + y >= grid.width)
    {
        return false;
    }
    for (int i = 0; i < quadCount; i++) {
        Quad* quad = &quads[quadPatch.quadIndices[i]];
        if (grid.get(quad->x, quad->y) != 0) {
            return false;
        }
    }
    return true;
}

void rasterize(int x, int y, QuadPatch &quadPatch, PackingGrid &grid, std::vector<Quad> &quads)
{
    int quadCount = quadPatch.quadIndices.size();
    for (int i = 0; i < quadCount; i++) {
        Quad* quad = &quads[quadPatch.quadIndices[i]];
        grid.set(quad->x, quad->y, quadPatch.index);
    }
}

bool areaComparator(QuadPatch &patchA, QuadPatch &patchB)
{
    return patchA.width * patchA.height > patchB.width * patchB.height;
}

void packPatch(QuadPatch &patch, PackingGrid &grid, std::vector<Quad> &quads)
{
    for (int i = 0; i < 10; i++) {
        for (int y = 0; y < grid.width; y++) {
            for (int x = 0; x < grid.width; x++) {
                if (rasterOk(x, y, patch, grid, quads)) {
                    rasterize(x, y, patch, grid, quads);
                    return;
                }
            }
        }
        grid.doubleSize();
    }
    std::cout << "unable to pack patch after 10 iterations!" << std::endl;
}

void packPatches(std::vector<QuadPatch> &quadPatches, PackingGrid &grid, std::vector<Quad> &quads)
{
    // Sort quad patches by area, decreasing
    std::sort (quadPatches.begin(), quadPatches.end(), areaComparator);

    int patchCount = quadPatches.size();

    for (int i = 0; i < patchCount; i++) {
        packPatch(quadPatches[i], grid, quads);
    }
}
