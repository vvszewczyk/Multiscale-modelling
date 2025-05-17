#pragma once
#include <QColor>

enum class State
{
    Empty,
    Occupied
};

class Cell
{
    State grainState;
    int grainID;
    QColor color;

  public:
    Cell(State s = State::Empty, int id = 0);
    State getState() const;
    int getGrainID() const;
    QColor colorForState() const;
    void setState(State s, int ID, const QColor &col);
    void reset();
};
