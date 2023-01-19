#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>


struct Tile
{
    int north;
    int west;
    int east;
    int south;
    bool inPlace;
};

class Tetravex
{
public:
    Tetravex() = default;
    Tetravex(std::string filename);
    ~Tetravex() = default;

    void readFromFile(std::string filename);
    void writeToFile(std::string filename) const;

    std::vector<Tile> getTiles() const { return tiles; }
    void setTiles(std::vector<Tile> tiles) { this->tiles = tiles; }
    int getSize() const { return size; }
    
    Tile getTile(int i) const { return tiles[i]; }
    Tile operator[](int i) const { return tiles[i]; }

    bool isSolved() const;

private:
    std::vector<Tile> tiles;
    int size;
};

std::ostream &operator<<(std::ostream &os, const Tetravex &tetravex);
