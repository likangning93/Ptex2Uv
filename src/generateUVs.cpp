#include "generateUVs.hpp"

void getSharedIndices(Quad* a, Quad* b, int& shared0, int &shared1) {
    shared0 = -1;
    shared1 = -1;
    for (uint ia = 0; ia < 4; ia++) {
        for (uint ib = 0; ib < 4; ib++) {
            if (a->indices[ia] == b->indices[ib]) {
                if (shared0 == -1) {
                    shared0 = a->indices[ia];
                } else {
                    shared1 = a->indices[ia];
                }
            }
        }
    }
}

int nextVertexIndexCcwise(Quad* quad, int vertexIndex) {
    int indexOfNext = 1;
    indexOfNext = quad->indices[1] == vertexIndex ? 2 : indexOfNext;
    indexOfNext = quad->indices[2] == vertexIndex ? 3 : indexOfNext;
    indexOfNext = quad->indices[3] == vertexIndex ? 0 : indexOfNext;

    return quad->indices[indexOfNext];
}

int nextVertexIndexCwise(Quad* quad, int vertexIndex) {
    int indexOfNext = 3;
    indexOfNext = quad->indices[1] == vertexIndex ? 0 : indexOfNext;
    indexOfNext = quad->indices[2] == vertexIndex ? 1 : indexOfNext;
    indexOfNext = quad->indices[3] == vertexIndex ? 2 : indexOfNext;

    return quad->indices[indexOfNext];
}


void generateUVs(tinyobj::attrib_t &attrib, std::vector<Quad> &quads,
                std::vector<QuadPatch> &quadPatches, PackingGrid &packingGrid,
                std::vector<tinyobj::index_t> &newIndices, tinyobj::attrib_t &newAttribs, int pixelWidth)
{
    // For each quad patch:
    uint quadPatchCount = quadPatches.size();

    bool hasNormals = attrib.normals.size() > 0;
    float inverseGridWidth = 1.0f / packingGrid.width;

    for (uint qp = 0; qp < quadPatchCount; qp++) {   
        QuadPatch *quadPatch = &quadPatches[qp];
        std::unordered_map<uint, uint> indexMap;

        // determine new vertex indices - no vertices may be shared between patches
        int nextAvailableIndex = newAttribs.vertices.size() / 3;
        uint quadCount = quadPatch->quadIndices.size();
        uint quadTexcoordIndices[] = {0, 0, 0, 0};
        for (uint q = 0; q < quadCount; q++) {
            Quad* quad = &quads[quadPatch->quadIndices[q]];
            for (uint i = 0; i < 4; i++) {
                uint ogIndex = quad->indices[i];
                if (indexMap.count(ogIndex)) {
                    tinyobj::index_t newIndex;
                    newIndex.vertex_index = indexMap[ogIndex];
                    newIndex.texcoord_index = newIndex.vertex_index;
                    if (hasNormals) {
                        newIndex.normal_index = newIndex.vertex_index;
                    } else {
                        newIndex.normal_index = -1;
                    }
                    newIndices.push_back(newIndex);
                    quadTexcoordIndices[i] = newIndex.texcoord_index;
                } else {
                    indexMap[ogIndex] = nextAvailableIndex;
                    tinyobj::index_t newIndex;
                    newIndex.vertex_index = nextAvailableIndex;
                    newIndex.texcoord_index = newIndex.vertex_index;
                    if (hasNormals) {
                        newIndex.normal_index = newIndex.vertex_index;
                    } else {
                        newIndex.normal_index = -1;
                    }
                    newIndices.push_back(newIndex);
                    quadTexcoordIndices[i] = newIndex.texcoord_index;

                    // look up positions
                    // look up normals... island scene does one-per-position
                    int vec3Index = ogIndex * 3;
                    newAttribs.vertices.push_back(attrib.vertices[vec3Index]);
                    newAttribs.vertices.push_back(attrib.vertices[vec3Index + 1]);
                    newAttribs.vertices.push_back(attrib.vertices[vec3Index + 2]);

                    if (hasNormals) {
                        newAttribs.normals.push_back(attrib.normals[vec3Index]);
                        newAttribs.normals.push_back(attrib.normals[vec3Index + 1]);
                        newAttribs.normals.push_back(attrib.normals[vec3Index + 2]);
                    }

                    newAttribs.texcoords.push_back(-1.0);
                    newAttribs.texcoords.push_back(-1.0);

                    nextAvailableIndex++;
                }
            }

            // Compute UVs:
            // - for start quad, determine each vertex's relative position in quad (0-1)
            //   - deterministic if there are neighbors on at least two adjacent sides
            //   - randomly pick for any directions this can't be determined
            //     * for single long strips, can result in strip that is "flipped" in texture
            //     * for single quads, UVs don't matter
            // - grow over patch
            //   - we guaranteed that quads in patches are adjacent to each other
            //   - so whenever texcoords are "undefined," use what's already existing
            uint patchX = quadPatch->x;
            uint patchY = quadPatch->y;
            int sharedIndex0, sharedIndex1;
            int texcoordIndex;
            int quadStartX = quad->x + quadPatch->x;
            int quadStartY = quad->y + quadPatch->y;
            if (q == 0) {
                // look up neighbors
                Quad* neighborInPatch0 = NULL;
                Quad* neighborInPatch1 = NULL;
                for (int n = 0; n < 4; n++) {
                    Quad* neighbor = &quads[quad->neighbors[n]];
                    if (neighbor->patchIndex == quad->patchIndex) {
                        if (neighborInPatch0 == NULL) {
                            neighborInPatch0 = neighbor;
                        } else {
                            neighborInPatch1 = neighbor;
                        }
                    }
                }

                // TODO: this code could probably be simplified a lot...
                if (neighborInPatch1 != NULL) {
                    // if 2 neighbors in patch, determine position for each vertex
                    getSharedIndices(neighborInPatch0, neighborInPatch1, sharedIndex0, sharedIndex1);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    
                    // Fully shared is 1 + 1
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * (quadStartX + 1);
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY + 1);

                    // travel CCWis
                    sharedIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * (quadStartX);
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY + 1);

                    sharedIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * quadStartX;
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * quadStartY;

                    sharedIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * (quadStartX + 1);
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY);
                    
                } else if (neighborInPatch0 != NULL) {
                    // if 1 neighbor in patch, determine one dimm, rando the other
                    getSharedIndices(neighborInPatch0, quad, sharedIndex0, sharedIndex1);

                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * (quadStartX + 1);
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY + 1);

                    sharedIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * quadStartX;
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY + 1);

                    sharedIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * quadStartX;
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * quadStartY;

                    sharedIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * (quadStartX + 1);
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY);
                } else {
                    // if no neighbors, rando all
                    sharedIndex0 = quad->indices[0];
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * (quadStartX);
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY);

                    sharedIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * (quadStartX + 1);
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY);

                    sharedIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * (quadStartX + 1);
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY + 1);

                    sharedIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                    texcoordIndex = indexMap[sharedIndex0] * 2;
                    newAttribs.texcoords[texcoordIndex    ] = inverseGridWidth * (quadStartX);
                    newAttribs.texcoords[texcoordIndex + 1] = inverseGridWidth * (quadStartY + 1);
                }

            } else {
                // one neighbor should already have computed tex coords,
                // so copy two of those and generate two new ones based on winding
                Quad* neighbourQuad = &quads[quadPatch->quadIndices[q - 1]];
                getSharedIndices(quad, neighbourQuad, sharedIndex0, sharedIndex1);
                
                int vertexIndex0 = nextVertexIndexCcwise(quad, sharedIndex0);
                if (vertexIndex0 == sharedIndex1) {
                    vertexIndex0 = nextVertexIndexCcwise(quad, vertexIndex0);
                }
                int texcoordIndex0 = indexMap[vertexIndex0] * 2;

                int vertexIndex1 = nextVertexIndexCcwise(quad, vertexIndex0);
                int texcoordIndex1 = indexMap[vertexIndex1] * 2;
                int texcoordIndex3 = indexMap[nextVertexIndexCwise(quad, vertexIndex0)] * 2; // shared with neighbourQuad
                int texcoordIndex2 = indexMap[nextVertexIndexCcwise(quad, vertexIndex1)] * 2; // shared with neighbourQuad
                if (quad->x > neighbourQuad->x) {
                    newAttribs.texcoords[texcoordIndex0    ] = inverseGridWidth * (quadStartX + 1);
                    newAttribs.texcoords[texcoordIndex0 + 1] = newAttribs.texcoords[texcoordIndex3 + 1];

                    newAttribs.texcoords[texcoordIndex1    ] = inverseGridWidth * (quadStartX + 1);
                    newAttribs.texcoords[texcoordIndex1 + 1] = newAttribs.texcoords[texcoordIndex2 + 1];
                } else {
                    newAttribs.texcoords[texcoordIndex0    ] = newAttribs.texcoords[texcoordIndex3];
                    newAttribs.texcoords[texcoordIndex0 + 1] = inverseGridWidth * (quadStartY + 1);

                    newAttribs.texcoords[texcoordIndex1    ] = newAttribs.texcoords[texcoordIndex2];
                    newAttribs.texcoords[texcoordIndex1 + 1] = inverseGridWidth * (quadStartY + 1);
                }
            }
        }
        
        // TODO: ability to shrink a packing grid
    }
}
