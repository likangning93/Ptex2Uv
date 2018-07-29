#ifndef BUILDGRAPH_H
#define BUILDGRAPH_H

#include "tiny_obj_loader.h"

#include <unordered_map>
#include <vector>

#include "Quad.hpp"

void buildGraph(std::vector<tinyobj::index_t> &indices, std::vector<Quad> &quads);

#endif
