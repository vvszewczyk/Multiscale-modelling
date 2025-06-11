#pragma once
#include "Cell.h"
#include <vector>

class Grid
{
    int cols, rows, depth;
    std::vector<Cell> cells;
    inline int BC(int x, int max) const;

  public:
    Grid(int cols, int rows, int depth);
    Cell &at(int x, int y, int z);
    const Cell &at(int x, int y, int z) const;

    int getCols() const;
    int getRows() const;
    int getDepth() const;
    int countEmpty() const;
    void reset();
};
