#include "PackingGrid.hpp"

#include <algorithm>

PackingGrid::PackingGrid(uint width)
{
    this->width = width;
    this->pixels = new std::vector<uint>();
    this->pixels->assign(width * width, 0);
}

PackingGrid::~PackingGrid()
{
    delete(this->pixels);
}

void PackingGrid::set(uint x, uint y, uint val)
{
    this->pixels->at(x + y * this->width) = val;
}

uint PackingGrid::get(uint x, uint y)
{
    return this->pixels->at(x + y * this->width);
}

void PackingGrid::doubleSize()
{
    std::vector<uint> *newVector = new std::vector<uint>();
    newVector->assign(width * width * 4, 0);
    uint oldWidth = this->width;
    this->width = width * 2;

    // copy to the top left
    for (uint y = 0; y < oldWidth; y++) {
        uint oldLineStart = y * oldWidth;
        uint newLineStart = y * this->width;
        std::copy(this->pixels->begin() + oldLineStart, this->pixels->begin() + oldLineStart + oldWidth, newVector->begin() + newLineStart);
    }

    delete(this->pixels);
    this->pixels = newVector;
}

void PackingGrid::print()
{
    for (uint y = 0; y < this->width; y++) {
        for (uint x = 0; x < this->width; x++) {
            std::cout << this->get(x, y);
        }
        std::cout << std::endl;
    }
}

void print(std::string str) {
    std::cout << str << std::endl;
}

void packingGridTests()
{
    print("***** START Grid Tests *****");

    print("test init:");
    
    PackingGrid grid(4);
    grid.print();

    print("");

    print("test set:");

    grid.set(0, 0, 1);
    grid.set(2, 0, 2);
    grid.set(1, 1, 3);
    grid.set(3, 1, 4);
    grid.set(0, 2, 5);
    grid.set(2, 2, 6);
    grid.set(1, 3, 7);
    grid.set(3, 3, 8);
    grid.print();

    print("");

    print("test doubleSize");

    grid.doubleSize();
    grid.print();

    print("");
    print("***** END Grid Tests *****");
}
