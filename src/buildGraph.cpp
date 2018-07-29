#include "buildGraph.hpp"

int cantorPair(int a, int b)
{
    int i0 = std::min(a, b);
    int i1 = std::max(a, b);

    return ((i0 + i1) * (i0 + i1 + 1) / 2) + i1;
}

void checkEdge(int i0, int i1, int quadIndex, Quad *thisQuad, std::vector<Quad> &quads, std::unordered_map<int, int> &edgeMap) {
    int key = cantorPair(i0, i1);
    
    // Check if the edge has been seen before
    if (edgeMap.count(key)) {
        int mappedIndex = edgeMap[key];
        // Update this quad and its edge neighbor, then remove from the map
        Quad* neighborQuad = &quads[mappedIndex];
        for (int i = 0; i < 4; i++) {
            if (thisQuad->edgeIndices[i] == key) {
                thisQuad->neighbors[i] = mappedIndex;
                break;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (neighborQuad->edgeIndices[i] == key) {
                neighborQuad->neighbors[i] = quadIndex;
                break;
            }
        }
        edgeMap.erase(key);
    } else {
        // If not, add this quad's index to the edge map
        edgeMap[key] = quadIndex;
    }
}

void buildGraph(std::vector<tinyobj::index_t> &indices, std::vector<Quad> &quads)
{
    std::unordered_map<int, int> edgeMap;
    int quadsCount = quads.size();

    for (int i = 0; i < quadsCount; i++)
    {
        Quad* quad = &quads[i];

        int index = i * 4;
        int v0 = indices[index].vertex_index;
        int v1 = indices[index + 1].vertex_index;
        int v2 = indices[index + 2].vertex_index;
        int v3 = indices[index + 3].vertex_index;

        quad->edgeIndices[0] = cantorPair(v0, v1);
        quad->edgeIndices[1] = cantorPair(v1, v2);
        quad->edgeIndices[2] = cantorPair(v2, v3);
        quad->edgeIndices[3] = cantorPair(v3, v0);

        checkEdge(v0, v1, i, quad, quads, edgeMap);
        checkEdge(v1, v2, i, quad, quads, edgeMap);
        checkEdge(v2, v3, i, quad, quads, edgeMap);
        checkEdge(v3, v0, i, quad, quads, edgeMap);
    }
}
