#include "GridWidget.h"
#include "Constants.h"
#include <QPainter>

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
}

void GridWidget::setShowGrid(bool value)
{
    this->showGrid = value;
    update();
}

void GridWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);

    int maxX = width() - 1;
    int maxY = height() - 1;

    if (!showGrid)
    {
        painter.drawRect(0, 0, maxX, maxY);
        return;
    }

    if (!showGrid)
    {
        painter.drawRect(0, 0, maxX, maxY);
        return;
    }

    for (int i = 0; i <= Config::gridCols; ++i)
    {
        int x = qMin(i * Config::cellSize, maxX);
        painter.drawLine(x, 0, x, maxY);
    }
    for (int j = 0; j <= Config::gridRows; ++j)
    {
        int y = qMin(j * Config::cellSize, maxY);
        painter.drawLine(0, y, maxX, y);
    }

    painter.drawRect(0, 0, maxX, maxY);
}
