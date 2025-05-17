#include "Simulation.h"
#include <QPoint>
#include <QRandomGenerator>
#include <vector>

static const QPoint VN[4] = {QPoint(1, 0), QPoint(-1, 0), QPoint(0, 1), QPoint(0, -1)};

Simulation::Simulation(int cols, int rows)
    : currentGrid(cols, rows), nextGrid(cols, rows), nextGrainID(1), iteration(0) {};

const Grid &Simulation::getGrid() const
{
    return this->currentGrid;
}

int Simulation::getIteration() const
{
    return this->iteration;
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

void Simulation::seedManual(int x, int y)
{
    Cell &c = currentGrid.at(x, y);

    if (c.getState() == State::Empty)
    {
        c.setState(State::Occupied, nextGrainID++, randomColor());
    }
}

void Simulation::seedRandom(int count)
{
    int w = currentGrid.getCols();
    int h = currentGrid.getRows();

    for (int i = 0; i < count; ++i)
    {
        int x = QRandomGenerator::global()->bounded(w);
        int y = QRandomGenerator::global()->bounded(h);
        seedManual(x, y);
    }
}

void Simulation::removeAt(int x, int y)
{
    Cell &clicked = currentGrid.at(x, y);
    if (clicked.getState() != State::Occupied)
    {
        return;
    }

    int targetID = clicked.getGrainID();
    int W = currentGrid.getCols();
    int H = currentGrid.getRows();

    for (int i = 0; i < W; ++i)
    {
        for (int j = 0; j < H; ++j)
        {
            Cell &c = currentGrid.at(i, j);
            if (c.getState() == State::Occupied && c.getGrainID() == targetID)
            {
                c.reset();
            }
        }
    }

    currentGrid.at(x, y).reset();
}

void Simulation::step()
{
    nextGrid.reset();
    int W = currentGrid.getCols();
    int H = currentGrid.getRows();

    for (int x = 0; x < W; ++x)
    {
        for (int y = 0; y < H; ++y)
        {
            Cell &c = currentGrid.at(x, y);
            if (c.getState() == State::Occupied)
            {
                nextGrid.at(x, y).setState(State::Occupied, c.getGrainID(), c.colorForState());
            }
            else
            {
                std::vector<Cell *> neighbours;

                for (int i = 0; i < 4; ++i)
                {
                    int nx = x + VN[i].x();
                    int ny = y + VN[i].y();
                    Cell &nc = currentGrid.at(nx, ny);

                    if (nc.getState() == State::Occupied)
                    {
                        neighbours.push_back(&nc);
                    }
                }

                if (!neighbours.empty())
                {
                    int idx = QRandomGenerator::global()->bounded(neighbours.size());
                    Cell *picked = neighbours[idx];
                    nextGrid.at(x, y).setState(State::Occupied, picked->getGrainID(), picked->colorForState());
                }
            }
        }
    }

    currentGrid = nextGrid;
    ++iteration;
}
