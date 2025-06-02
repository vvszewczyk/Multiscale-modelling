#pragma once
#include "Grid.h"

enum class NeighbourhoodType
{
    VonNeumann,
    Moore,
    HexagonalRandom
};

class Simulation
{
    Grid currentGrid;
    Grid nextGrid;
    int nextGrainID;
    int iteration;
    NeighbourhoodType neighbourhoodType;

    QColor randomColor() const;

  public:
    Simulation(int cols, int rows, int depth);
    void step();
    void reset();

    void seedManual(int x, int y, int z);
    void seedRandom(int N);
    void seedRegular(int stepX, int stepY, int stepZ);

    void removeAt(int x, int y, int z);

    const Grid &getGrid() const;
    int getIteration() const;

    void setNeighbourhoodType(NeighbourhoodType n);
    NeighbourhoodType getNeighbourhoodType() const;
};
