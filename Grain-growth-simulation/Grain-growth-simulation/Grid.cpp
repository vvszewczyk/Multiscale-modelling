#include "Grid.h"
#include "Cell.h"

Grid::Grid(int c, int r, int d) : cols(c), rows(r), depth(d), cells(c * r * d) {};

inline int Grid::BC(int x, int max) const
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

Cell &Grid::at(int x, int y, int z)
{
    x = BC(x, cols);
    y = BC(y, rows);
    z = BC(z, depth);
    return cells[(z * rows + y) * cols + x];
}

const Cell &Grid::at(int x, int y, int z) const
{
    x = BC(x, cols);
    y = BC(y, rows);
    z = BC(z, depth);
    return cells[(z * rows + y) * cols + x];
}

int Grid::getCols() const
{
    return this->cols;
}

int Grid::getRows() const
{
    return this->rows;
}

int Grid::getDepth() const
{
    return this->depth;
}

void Grid::reset()
{
    for (Cell &c : cells)
    {
        c.reset();
    }
}

int Grid::countEmpty() const
{
    int counter = 0;
    for (const Cell &c : cells)
    {
        if (c.getState() == State::Empty)
        {
            ++counter;
        }
    }

    return counter;
}
