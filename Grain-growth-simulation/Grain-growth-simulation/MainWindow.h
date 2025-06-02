#pragma once
#include <GridWidget.h>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QVBoxLayout>
#include <QtWidgets/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    GridWidget *gridWidget;
    Simulation *sim;
    QTimer *timer;
    QPushButton *startButton;
    QPushButton *resetButton;
    QCheckBox *gridToggle;
    QCheckBox *ereaseToggle;
    QLabel *iterationLabel;

    QLabel *layerLabel;
    QSlider *layerSlider;

    QPushButton *placeGrains;

    QLabel *neighbourhoodLabel;
    QComboBox *neighbourhood;

    void setupUI();
    void setupLayout();
    void setupConnections();
    void updateIterationLabel();

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void onStartClicked();
    void onResetClicked();
    void onStep();
    void onLayerChanged(int newZ);
    void onPlaceGrainsClicked();
    void onNeighbourhoodChanged(int index);
};
