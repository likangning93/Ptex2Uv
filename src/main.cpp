#include <iostream>

#include "buildGraph.hpp"
#include "buildPatches.hpp"
#include "Quad.hpp"
#include "QuadPatch.hpp"
#include "PackingGrid.hpp"
#include "packPatches.hpp"
#include "generateUVs.hpp"
#include "obj_writer.h"

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

        PackingGrid packingGrid(sqrt(quads.size()) + 1);

        packPatches(quadPatches, packingGrid, quads);

        std::cout << "PACKING..." << std::endl;
        packingGrid.print();

        std::cout << "PACKING DONE" << std::endl;

        tinyobj::attrib_t newAttribs;
        tinyobj::mesh_t newMesh;

        generateUVs(attrib, quads, quadPatches, packingGrid, newMesh.indices, newAttribs, 1024);

        uint faceCount = newMesh.indices.size() / 4;
        for (uint i = 0; i < faceCount; i++) {
            newMesh.num_face_vertices.push_back('\004');
            newMesh.material_ids.push_back(-1);
            newMesh.smoothing_group_ids.push_back(0);
        }

        tinyobj::shape_t newShape;
        newShape.mesh = newMesh;
        std::vector<tinyobj::shape_t> newShapes;
        newShapes.push_back(newShape);

        std::cout << "GENERATE UVS DONE" << std::endl;

        std::string output = inputfile.substr(0, inputfile.length() - 4) + "_test.obj";

        std::cout << "writing to " << output << std::endl;
        WriteObj(output, newAttribs, newShapes, materials);

        std::cout << "done" << std::endl;
    }
}
