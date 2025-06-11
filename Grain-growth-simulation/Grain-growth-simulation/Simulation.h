#pragma once
#include "Grid.h"
#include <QRandomGenerator>
#include <QVector3D>
#include <vector>

enum class NeighbourhoodType
{
    VonNeumann,
    Moore,
    HexagonalRandom
};

struct NeighbourhoodDefinition
{
    const QVector3D *offsets;
    int count;
};

class Simulation
{
    Grid currentGrid;
    Grid nextGrid;
    int nextGrainID;
    int iteration;
    double jgb;
    double kt;
    double mcSteps;
    NeighbourhoodType neighbourhoodType;

    QColor randomColor() const;

  public:
    Simulation(int cols, int rows, int depth);
    void step();
    void reset();

    void mcStep();
    double localEnergy(int x, int y, int z) const;

    void seedManual(int x, int y, int z);
    void seedRandom(int N);
    void seedRegular(int stepX, int stepY, int stepZ);

    void removeAt(int x, int y, int z);
    void removeGrainByID(int grainID);

    const Grid &getGrid() const;
    int getIteration() const;

    void setNeighbourhoodType(NeighbourhoodType n);
    NeighbourhoodType getNeighbourhoodType() const;

  public slots:
    void setJgb(double value);
    void setKt(double value);
};
