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
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QtWidgets/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    GridWidget *gridWidget;
    Simulation *sim;
    QTimer *timer;

    QLabel *simulationLabel;
    QLabel *layerLabel;
    QLabel *iterationLabel;
    QLabel *grainsLabel;
    QLabel *neighbourhoodLabel;
    QLabel *cellSizeLabel;

    QPushButton *startButton;
    QPushButton *resetButton;
    QPushButton *randomGrainsButton;
    QPushButton *regularGrainsButton;

    QCheckBox *gridToggle;
    QCheckBox *ereaseToggle;

    QSlider *layerSlider;

    QComboBox *neighbourhoodCombo;
    QComboBox *cellSizeCombo;

    QSpinBox *randomCountSpin;
    QSpinBox *regularStrideSpin;

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

    void onCellSizeChanged(int index);
    void onRandomCountChanged(int newVal);
    void onRegularStrideChanged(int newVal);

    void onRandomGrainsClicked();
    void onRegularGrainsClicked();
    void onNeighbourhoodChanged(int index);
};
