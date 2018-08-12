#ifndef GENERATE_UVS_H
#define GENERATE_UVS_H

#include "tiny_obj_loader.h"

#include <unordered_map>

#include "PackingGrid.hpp"
#include "Quad.hpp"
#include "QuadPatch.hpp"

void generateUVs(tinyobj::attrib_t &attrib, std::vector<Quad> &quads,
                std::vector<QuadPatch> &quadPatches, PackingGrid &packingGrid,
                std::vector<tinyobj::index_t> &newIndices, tinyobj::attrib_t &newAttribs, int pixelWidth);

#endif