#ifndef PACKINGGRID_H
#define PACKINGGRID_H

#include <vector>
#include <iostream>
#include <string>

// 3D bitmask for things like uniform grid sampling
class PackingGrid
{
public:
    PackingGrid(uint width);
    ~PackingGrid();

    uint width;

    void set(uint x, uint y, uint val);

    uint get(uint x, uint y);
    
    void doubleSize();

    void print();

private:
    std::vector<uint> *pixels;
};

void packingGridTests();

#endif
