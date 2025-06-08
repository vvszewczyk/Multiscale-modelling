#pragma once
#include <GridWidget.h>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
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
    QLabel *layersCountLabel;
    QLabel *iterationLabel;
    QLabel *grainsLabel;
    QLabel *neighbourhoodLabel;
    QLabel *cellSizeLabel;

    QPushButton *startButton;
    QPushButton *resetButton;
    QPushButton *randomGrainsButton;
    QPushButton *regularGrainsButton;
    QPushButton *ereaseGrainButton;
    QPushButton *savePNGButton;
    QPushButton *saveOvitoButton;

    QCheckBox *gridToggle;
    QCheckBox *ereaseToggle;

    QSlider *layerSlider;

    QComboBox *neighbourhoodCombo;
    QComboBox *cellSizeCombo;
    QComboBox *ereaseGrainCombo;

    QSpinBox *randomCountSpin;
    QSpinBox *regularStrideSpin;
    QSpinBox *layersCountSpin;

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
    void onLayersCountChanged(int newDepth);

    void onRandomGrainsClicked();
    void onRegularGrainsClicked();
    void onNeighbourhoodChanged(int index);
    void onEreaseGrainButtonClicked();
    void onSavePNGClicked();

    void refreshEreaseCombo();
};
