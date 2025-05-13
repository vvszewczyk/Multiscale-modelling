#pragma once
#include <QColor>

enum class State
{
    Dead,
    Allive
};

class Cell
{
    State state;

  public:
    Cell(State s = State::Dead);
    State state() const;
    void setState(State);
    QColor colorForState() const;
};
