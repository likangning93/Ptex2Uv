#include <iostream>

#include "buildGraph.hpp"
#include "Quad.hpp"

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
        /*
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }*/
    }
}
