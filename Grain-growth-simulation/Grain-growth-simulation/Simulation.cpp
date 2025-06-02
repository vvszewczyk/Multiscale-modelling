#include "Simulation.h"
#include <QRandomGenerator>
#include <QVector3D>
#include <vector>

static const QVector3D VN[6] = {QVector3D(1, 0, 0),  QVector3D(-1, 0, 0), QVector3D(0, 1, 0),
                                QVector3D(0, -1, 0), QVector3D(0, 0, 1),  QVector3D(0, 0, -1)};

static const QVector3D MOORE[26] = {
    QVector3D(-1, -1, -1), QVector3D(-1, -1, 0), QVector3D(-1, -1, 1), QVector3D(-1, 0, -1), QVector3D(-1, 0, 0),
    QVector3D(-1, 0, 1),   QVector3D(-1, 1, -1), QVector3D(-1, 1, 0),  QVector3D(-1, 1, 1),  QVector3D(0, -1, -1),
    QVector3D(0, -1, 0),   QVector3D(0, -1, 1),  QVector3D(0, 0, -1),  QVector3D(0, 0, 1),   QVector3D(0, 1, -1),
    QVector3D(0, 1, 0),    QVector3D(0, 1, 1),   QVector3D(1, -1, -1), QVector3D(1, -1, 0),  QVector3D(1, -1, 1),
    QVector3D(1, 0, -1),   QVector3D(1, 0, 0),   QVector3D(1, 0, 1),   QVector3D(1, 1, -1),  QVector3D(1, 1, 0),
    QVector3D(1, 1, 1)};

Simulation::Simulation(int cols, int rows, int depth)
    : currentGrid(cols, rows, depth), nextGrid(cols, rows, depth), nextGrainID(1), iteration(0),
      neighbourhoodType(NeighbourhoodType::VonNeumann) {};

const Grid &Simulation::getGrid() const
{
    return this->currentGrid;
}

int Simulation::getIteration() const
{
    return this->iteration;
}

void Simulation::setNeighbourhoodType(NeighbourhoodType n)
{
    neighbourhoodType = n;
}

NeighbourhoodType Simulation::getNeighbourhoodType() const
{
    return neighbourhoodType;
}

void Simulation::reset()
{
    currentGrid.reset();
    nextGrid.reset();
    nextGrainID = 1;
    iteration = 0;
}

QColor Simulation::randomColor() const
{
    int r = QRandomGenerator::global()->bounded(256);
    int g = QRandomGenerator::global()->bounded(256);
    int b = QRandomGenerator::global()->bounded(256);

    return QColor(r, g, b);
}

void Simulation::seedManual(int x, int y, int z)
{
    Cell &c = currentGrid.at(x, y, z);

    if (c.getState() == State::Empty)
    {
        c.setState(State::Occupied, nextGrainID++, randomColor());
    }
}

void Simulation::seedRandom(int count)
{
    int w = currentGrid.getCols();
    int h = currentGrid.getRows();
    int d = currentGrid.getDepth();

    for (int i = 0; i < count; ++i)
    {
        int x = QRandomGenerator::global()->bounded(w);
        int y = QRandomGenerator::global()->bounded(h);
        int z = QRandomGenerator::global()->bounded(d);
        seedManual(x, y, z);
    }
}

void Simulation::removeAt(int x, int y, int z)
{
    Cell &clicked = currentGrid.at(x, y, z);
    if (clicked.getState() != State::Occupied)
    {
        return;
    }

    int targetID = clicked.getGrainID();
    int W = currentGrid.getCols();
    int H = currentGrid.getRows();
    int D = currentGrid.getDepth();

    for (int i = 0; i < W; ++i)
    {
        for (int j = 0; j < H; ++j)
        {
            for (int k = 0; k < D; ++k)
            {
                Cell &c = currentGrid.at(i, j, k);
                if (c.getState() == State::Occupied && c.getGrainID() == targetID)
                {
                    c.reset();
                }
            }
        }
    }

    currentGrid.at(x, y, z).reset();
}

void Simulation::step()
{
    nextGrid.reset();
    int W = currentGrid.getCols();
    int H = currentGrid.getRows();
    int D = currentGrid.getDepth();

    const QVector3D *offsets = nullptr;
    int offsetCount = 0;

    if (neighbourhoodType == NeighbourhoodType::VonNeumann)
    {
        offsets = VN;
        offsetCount = 6;
    }
    else
    {
        offsets = MOORE;
        offsetCount = 26;
    }

    for (int x = 0; x < W; ++x)
    {
        for (int y = 0; y < H; ++y)
        {
            for (int z = 0; z < D; ++z)
            {
                Cell &c = currentGrid.at(x, y, z);
                if (c.getState() == State::Occupied)
                {
                    nextGrid.at(x, y, z).setState(State::Occupied, c.getGrainID(), c.colorForState());
                }
                else
                {
                    std::vector<Cell *> neighbours;
                    neighbours.reserve(offsetCount);

                    for (int i = 0; i < offsetCount; ++i)
                    {
                        int nx = x + offsets[i].x();
                        int ny = y + offsets[i].y();
                        int nz = z + offsets[i].z();
                        Cell &nc = currentGrid.at(nx, ny, nz);

                        if (nc.getState() == State::Occupied)
                        {
                            neighbours.push_back(&nc);
                        }
                    }

                    if (!neighbours.empty())
                    {
                        int idx = QRandomGenerator::global()->bounded(neighbours.size());
                        Cell *picked = neighbours[idx];
                        nextGrid.at(x, y, z).setState(State::Occupied, picked->getGrainID(), picked->colorForState());
                    }
                }
            }
        }
    }

    currentGrid = nextGrid;
    ++iteration;
}
