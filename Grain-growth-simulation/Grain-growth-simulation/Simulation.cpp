#include "Simulation.h"
#include "Constants.h"
#include <Grid.h>

Simulation::Simulation(int cols, int rows, int depth)
    : currentGrid(cols, rows, depth), nextGrid(cols, rows, depth), nextGrainID(1), iteration(0),
      neighbourhoodType(NeighbourhoodType::VonNeumann), jgb(1.0), kt(0.1), mcSteps(0.0) {};

static NeighbourhoodDefinition getNeighborhood(NeighbourhoodType t)
{
    switch (t)
    {
    case NeighbourhoodType::VonNeumann:
        return {Config::VN, 6};

    case NeighbourhoodType::Moore:
        return {Config::MOORE, 26};

    case NeighbourhoodType::HexagonalRandom:
        return {nullptr, 6 + 2};
    }

    return {Config::VN, 6};
}

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

void Simulation::setJgb(double value)
{
    this->jgb = value;
}

void Simulation::setKt(double value)
{
    this->kt = value;
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
    Cell &clicked = currentGrid.at(x, y, z);

    if (clicked.getState() == State::Empty)
    {
        clicked.setState(State::Occupied, nextGrainID++, randomColor());
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

void Simulation::seedRegular(int stepX, int stepY, int stepZ)
{
    int w = currentGrid.getCols();
    int h = currentGrid.getRows();
    int d = currentGrid.getDepth();

    for (int z = 0; z < d; z += stepZ)
    {
        for (int y = 0; y < h; y += stepY)
        {
            for (int x = 0; x < w; x += stepX)
            {
                Cell &c = currentGrid.at(x, y, z);
                if (c.getState() == State::Empty)
                {
                    c.setState(State::Occupied, nextGrainID++, randomColor());
                }
            }
        }
    }
}

void Simulation::removeAt(int x, int y, int z)
{
    Cell &clicked = currentGrid.at(x, y, z);
    if (clicked.getState() == State::Empty)
    {
        return;
    }

    int targetID = clicked.getGrainID();
    int w = currentGrid.getCols();
    int h = currentGrid.getRows();
    int d = currentGrid.getDepth();

    for (int i = 0; i < w; ++i)
    {
        for (int j = 0; j < h; ++j)
        {
            for (int k = 0; k < d; ++k)
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

void Simulation::removeGrainByID(int grainID)
{
    int W = currentGrid.getCols();
    int H = currentGrid.getRows();
    int D = currentGrid.getDepth();

    for (int x = 0; x < W; ++x)
    {
        for (int y = 0; y < H; ++y)
        {
            for (int z = 0; z < D; ++z)
            {
                Cell &c = currentGrid.at(x, y, z);
                if (c.getState() == State::Occupied && c.getGrainID() == grainID)
                {
                    c.reset();
                }
            }
        }
    }
}

void Simulation::step()
{
    nextGrid.reset();
    int W = currentGrid.getCols();
    int H = currentGrid.getRows();
    int D = currentGrid.getDepth();

    const QVector3D *offsets = nullptr;
    int offsetCount = 0;
    NeighbourhoodDefinition def = getNeighborhood(neighbourhoodType);

    offsets = def.offsets;
    offsetCount = def.count;

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

                    if (neighbourhoodType == NeighbourhoodType::VonNeumann ||
                        neighbourhoodType == NeighbourhoodType::Moore)
                    {
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
                    }
                    else if (neighbourhoodType == NeighbourhoodType::HexagonalRandom)
                    {
                        int whichHex = QRandomGenerator::global()->bounded(2); // 0/1 in XY
                        offsets = (whichHex == 0 ? Config::HEXAGON_A : Config::HEXAGON_B);
                        for (int i = 0; i < offsetCount - 2; ++i)
                        {
                            int nx = x + offsets[i].x();
                            int ny = y + offsets[i].y();
                            int nz = z + offsets[i].z();
                            Cell &nc = currentGrid.at(nx, ny, nz);

                            if (nc.getState() == State::Occupied)
                            {
                                neighbours.push_back(&nc);
                            }

                            Cell &up = currentGrid.at(x, y, z + 1);
                            if (up.getState() == State::Occupied)
                            {
                                neighbours.push_back(&up);
                            }
                            Cell &dn = currentGrid.at(x, y, z - 1);
                            if (dn.getState() == State::Occupied)
                            {
                                neighbours.push_back(&dn);
                            }
                        }
                    }

                    if (!neighbours.empty())
                    {
                        // Neighbour frequency
                        std::unordered_map<int, int> freq;
                        freq.reserve(neighbours.size());

                        for (Cell *nc : neighbours)
                        {
                            freq[nc->getGrainID()]++;
                        }

                        // Find max frequency
                        int maxCount = 0;
                        for (std::pair<int, int> pair : freq)
                        {
                            if (pair.second > maxCount)
                            {
                                maxCount = pair.second;
                            }
                        }

                        // Find all neighbours with the same frequency as max
                        std::vector<int> candidates;
                        for (std::pair<int, int> pair : freq)
                        {
                            if (pair.second == maxCount)
                            {
                                candidates.push_back(pair.first);
                            }
                        }

                        // Draw if more candidates
                        int chosenGrainID;
                        if (candidates.size() == 1)
                        {
                            chosenGrainID = candidates[0];
                        }
                        else
                        {
                            int rnd = QRandomGenerator::global()->bounded(candidates.size());
                            chosenGrainID = candidates[rnd];
                        }

                        QColor chosenColor = Qt::white;
                        for (Cell *nc : neighbours)
                        {
                            if (nc->getGrainID() == chosenGrainID)
                            {
                                chosenColor = nc->colorForState();
                            }
                        }

                        nextGrid.at(x, y, z).setState(State::Occupied, chosenGrainID, chosenColor);
                    }
                }
            }
        }
    }

    currentGrid = nextGrid;
    ++iteration;
}

double Simulation::localEnergy(int x, int y, int z) const
{
    NeighbourhoodDefinition def = getNeighborhood(neighbourhoodType);

    const int W = currentGrid.getCols();
    const int H = currentGrid.getRows();
    const int D = currentGrid.getDepth();

    const Cell &c = currentGrid.at(x, y, z);
    int grainID = c.getGrainID();
    double E = 0;
    if (neighbourhoodType == NeighbourhoodType::VonNeumann || neighbourhoodType == NeighbourhoodType::Moore)
    {
        for (int i = 0; i < def.count; ++i)
        {
            int nx = x + def.offsets[i].x();
            int ny = y + def.offsets[i].y();
            int nz = z + def.offsets[i].z();
            const Cell &n = currentGrid.at(nx, ny, nz);
            if (n.getState() == State::Occupied && n.getGrainID() != grainID)
            {
                E += 1;
            }
        }
    }
    else
    {
        int which = QRandomGenerator::global()->bounded(2);
        const QVector3D *hex = (which == 0 ? Config::HEXAGON_A : Config::HEXAGON_B);

        for (int i = 0; i < def.count - 2; ++i)
        {
            int nx = x + hex[i].x();
            int ny = y + hex[i].y();
            const Cell &n = currentGrid.at(nx, ny, z);
            if (n.getState() == State::Occupied && n.getGrainID() != grainID)
            {
                E += 1;
            }
        }
        {
            const Cell &up = currentGrid.at(x, y, z + 1);
            if (up.getState() == State::Occupied && up.getGrainID() != grainID)
            {
                E += 1;
            }
            const Cell &dn = currentGrid.at(x, y, z - 1);
            if (dn.getState() == State::Occupied && dn.getGrainID() != grainID)
            {
                E += 1;
            }
        }
    }
    return jgb * E;
}

void Simulation::mcStep()
{
    int W = currentGrid.getCols();
    int H = currentGrid.getRows();
    int D = currentGrid.getDepth();

    std::vector<int> idx(W * H * D);
    std::iota(idx.begin(), idx.end(), 0);
    std::shuffle(idx.begin(), idx.end(), *QRandomGenerator::global());

    for (int lin : idx)
    {
        int z = lin / (W * H);
        int r = lin % (W * H);
        int y = r / W;
        int x = r % W;

        Cell &c = currentGrid.at(x, y, z);
        if (c.getState() != State::Occupied)
        {
            continue;
        }

        double E0 = localEnergy(x, y, z);
        NeighbourhoodDefinition def = getNeighborhood(neighbourhoodType);
        int nx, ny, nz;
        if (neighbourhoodType == NeighbourhoodType::VonNeumann || neighbourhoodType == NeighbourhoodType::Moore)
        {
            int w = QRandomGenerator::global()->bounded(def.count);
            nx = x + def.offsets[w].x();
            ny = y + def.offsets[w].y();
            nz = z + def.offsets[w].z();
        }
        else
        {
            int which = QRandomGenerator::global()->bounded(2);
            const QVector3D *hex = (which == 0 ? Config::HEXAGON_A : Config::HEXAGON_B);
            int w = QRandomGenerator::global()->bounded(6);
            nx = x + static_cast<int>(hex[w].x());
            ny = y + static_cast<int>(hex[w].y());
            nz = z;

            if (QRandomGenerator::global()->bounded(2) == 0)
            {
                nz = z + 1;
            }
            else
            {
                nz = z - 1;
            }
        }

        int oldID = c.getGrainID();
        QColor oldCol = c.colorForState();

        Cell &n = currentGrid.at(nx, ny, nz);
        int newID = n.getGrainID();
        QColor newCol = n.colorForState();
        c.setState(State::Occupied, newID, newCol);

        double E1 = localEnergy(x, y, z);
        double deltaE = E1 - E0;

        if (deltaE > 0)
        {
            double p = (deltaE <= 0) ? 1.0 : std::exp(-deltaE / kt);
            if (QRandomGenerator::global()->generateDouble() >= p)
            {
                c.setState(State::Occupied, oldID, oldCol);
            }
        }
    }
}
