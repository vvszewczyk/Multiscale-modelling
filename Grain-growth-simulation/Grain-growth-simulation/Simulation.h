#pragma once
#include "Grid.h"
class Simulation
{
    Grid currentGrid;
    Grid nextGrid;
    int nextGrainID;
    int iteration;

    QColor randomColor() const;

  public:
    Simulation(int cols, int rows);
    void step();
    void reset();

    void seedRandom(int N);
    void seedManual(int x, int y);
    void removeAt(int x, int y);

    const Grid &getGrid() const;
    int getIteration() const;
};
