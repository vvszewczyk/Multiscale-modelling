#include "Cell.h"

Cell::Cell(State s, int id) : grainState(s), grainID(id), color(Qt::white) {};

State Cell::getState() const
{
    return this->grainState;
}

int Cell::getGrainID() const
{
    return this->grainID;
}

QColor Cell::colorForState() const
{
    return (grainState == State::Occupied ? this->color : Qt::white);
}

void Cell::setState(State s, int ID, const QColor &col)
{
    this->grainState = s;
    this->grainID = ID;
    this->color = col;
}

void Cell::reset()
{
    this->grainState = State::Empty;
    this->grainID = 0;
    this->color = Qt::white;
}
