#pragma once
#include "Cell.h"
#include <vector>

class Grid
{
    int cols, rows;
    std::vector<Cell> cells;
    inline int wrap(int x, int max) const;

  public:
    Grid(int cols, int rows);
    int getCols() const;
    int getRows() const;
    Cell &at(int x, int y);
    const Cell &at(int x, int y) const;
    void reset();
};
