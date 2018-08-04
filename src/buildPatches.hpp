#ifndef BUILDPATCHES_H
#define BUILDPATCHES_H

#include <algorithm>
#include <vector>

#include "Quad.hpp"
#include "QuadPatch.hpp"

void buildPatches(std::vector<Quad> &quads, std::vector<QuadPatch> &quadPatches, int dimensionLimit);

#endif
