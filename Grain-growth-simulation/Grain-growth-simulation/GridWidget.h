#pragma once

#include "Simulation.h"
#include <QMouseEvent>
#include <QWidget>

class GridWidget : public QWidget
{
    Q_OBJECT
    Simulation *sim;
    bool showGrid;
    bool ereaseMode;
    int currentZ;

  public:
    explicit GridWidget(QWidget *parent = nullptr);
    void setSimulation(Simulation *s);
    void setShowGrid(bool value);
    void setEreaseMode(bool value);
    void setLayer(int z);

  signals:
    void cellRemoved(int x, int y, int z);

  protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};
