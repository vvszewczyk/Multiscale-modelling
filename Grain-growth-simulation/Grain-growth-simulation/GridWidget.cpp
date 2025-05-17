#include "GridWidget.h"
#include "Constants.h"
#include <QPainter>

GridWidget::GridWidget(QWidget *parent) : QWidget(parent), showGrid(true), sim(nullptr) {};

void GridWidget::setSimulation(Simulation *s)
{
    this->sim = s;
}

void GridWidget::setShowGrid(bool value)
{
    this->showGrid = value;
    update();
}

void GridWidget::setEreaseMode(bool value)
{
    ereaseMode = value;
}

void GridWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Drawing simulation
    if (sim)
    {
        const Grid &grid = sim->getGrid();
        for (int x = 0; x < grid.getCols(); ++x)
        {
            for (int y = 0; y < grid.getRows(); ++y)
            {
                painter.fillRect(x * Config::cellSize, y * Config::cellSize, Config::cellSize, Config::cellSize,
                                 grid.at(x, y).colorForState());
            }
        }
    }

    // Drawing grid
    if (showGrid)
    {
        painter.setPen(Qt::black);
        int maxX = width() - 1;
        int maxY = height() - 1;

        for (int i = 0; i <= Config::gridCols; ++i)
        {
            int xx = qMin(i * Config::cellSize, maxX);
            painter.drawLine(xx, 0, xx, maxY);
        }
        for (int j = 0; j <= Config::gridRows; ++j)
        {
            int yy = qMin(j * Config::cellSize, maxY);
            painter.drawLine(0, yy, maxX, yy);
        }

        painter.drawRect(0, 0, maxX, maxY);
    }
}

void GridWidget::mousePressEvent(QMouseEvent *event)
{
    if (!sim)
    {
        return;
    }

    QPoint p = event->position().toPoint();

    int x = p.x() / Config::cellSize;
    int y = p.y() / Config::cellSize;

    if (!ereaseMode)
    {
        sim->seedManual(x, y);
    }
    else
    {
        sim->removeAt(x, y);
    }

    update();
}
