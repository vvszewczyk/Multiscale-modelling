#pragma once
#include "Cell.h"
#include <vector>

class Grid
{
    int cols, rows;
    std::vector<Cell> cells;

  public:
    Grid(int cols, int rows);
    Cell &at(int x, int y);
    int width() const;
    int height() const;
};
