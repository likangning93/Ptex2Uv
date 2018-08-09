#ifndef PACKPATCHES_H
#define PACKPATCHES_H

#include <algorithm>
#include <vector>

#include "Quad.hpp"
#include "QuadPatch.hpp"
#include "PackingGrid.hpp"

void packPatches(std::vector<QuadPatch> &quadPatches, PackingGrid &grid, std::vector<Quad> &quads);

#endif
