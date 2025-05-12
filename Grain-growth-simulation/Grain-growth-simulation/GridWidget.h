#pragma once

#include <QWidget>

class GridWidget : public QWidget
{
    Q_OBJECT
    bool showGrid;

  public:
    explicit GridWidget(QWidget *parent = nullptr);
    void setShowGrid(bool value);

  protected:
    void paintEvent(QPaintEvent *event) override;
};
