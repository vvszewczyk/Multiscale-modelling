#pragma once
#include "Grid.h"
class Simulation
{
    Grid grid;

  public:
    Simulation(int cols, int rows);
    void step();
    const Grid &getGrid() const;
};
