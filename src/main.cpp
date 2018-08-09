#include <iostream>

#include "buildGraph.hpp"
#include "buildPatches.hpp"
#include "Quad.hpp"
#include "QuadPatch.hpp"
#include "PackingGrid.hpp"
#include "packPatches.hpp"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc after includes that also use tiny_obj_loader.h
#include "tiny_obj_loader.h"

int main(int argc, char *argv[])
{
    // Just some sample code from the tinyobj site

    std::string inputfile = argv[1];
    std::cout << "loading " << inputfile << std::endl;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), NULL, false);

    if (!err.empty())
    { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }

    if (!ret)
    {
        exit(1);
    }

    std::cout << "SUCCESS!" << std::endl;
    std::cout << "shapes.size(): " << shapes.size() << std::endl;

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        tinyobj::shape_t shape = shapes[s];
        std::vector<Quad> quads;
        quads.resize(shape.mesh.indices.size() / 4);
        buildGraph(shape.mesh.indices, quads);

        std::cout << "GRAPH DONE" << std::endl;

        std::vector<QuadPatch> quadPatches;
        buildPatches(quads, quadPatches, 50);

        std::cout << "PATCHES DONE" << std::endl;

        PackingGrid grid(sqrt(quads.size()) + 1);

        packPatches(quadPatches, grid, quads);

        std::cout << "PACKING DONE" << std::endl;
        grid.print();
    }
}
