#include "GridWidget.h"

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
    if (!showGrid)
    {
        return;
    }

    QPainter painter(this);
    painter.setPen(Qt::black);

    const int cellSize = 10;
    int cols = std::ceil(width() / double(cellSize));
    int rows = std::ceil(height() / double(cellSize));
    int maxX = width() - 1;
    int maxY = height() - 1;

    for (int i = 0; i <= cols; ++i)
    {
        int x = qMin(i * cellSize, maxX);
        painter.drawLine(x, 0, x, maxY);
    }
    for (int j = 0; j <= rows; ++j)
    {
        int y = qMin(j * cellSize, maxY);
        painter.drawLine(0, y, maxX, y);
    }

    painter.drawRect(0, 0, maxX, maxY);
}
