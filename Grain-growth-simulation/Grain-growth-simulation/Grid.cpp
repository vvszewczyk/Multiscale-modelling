#include "Grid.h"
#include "Cell.h"

Grid::Grid(int c, int r) : cols(c), rows(r), cells(c * r) {};

int Grid::wrap(int x, int max) const
{
    if (x < 0)
    {
        return max - 1;
    }
    if (x >= max)
    {
        return 0;
    }
    return x;
}

Cell &Grid::at(int x, int y)
{
    x = wrap(x, cols);
    y = wrap(y, rows);
    return cells[y * cols + x];
}

const Cell &Grid::at(int x, int y) const
{
    x = wrap(x, cols);
    y = wrap(y, rows);
    return cells[y * cols + x];
}

int Grid::getCols() const
{
    return this->cols;
}

int Grid::getRows() const
{
    return this->rows;
}

void Grid::reset()
{
    for (Cell &c : cells)
    {
        c.reset();
    }
}
