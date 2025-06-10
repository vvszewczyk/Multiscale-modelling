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
#include <QtCharts>
#include <QtWidgets/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QStackedWidget *contentStack;
    GridWidget *gridWidget;
    QWidget *statsWidget;

    Simulation *sim;
    QTimer *timer;

    QLabel *simulationLabel;
    QLabel *layerLabel;
    QLabel *layersCountLabel;
    QLabel *iterationLabel;
    QLabel *grainsLabel;
    QLabel *neighbourhoodLabel;
    QLabel *cellSizeLabel;

    QLabel *totalGrainsLabel;
    QLabel *occupiedFractionLabel;
    QLabel *avgSizeLabel;
    QLabel *stdDevLabel;
    QLabel *iterationValueLabel;

    QPushButton *startButton;
    QPushButton *resetButton;
    QPushButton *randomGrainsButton;
    QPushButton *regularGrainsButton;
    QPushButton *ereaseGrainButton;
    QPushButton *savePNGButton;
    QPushButton *saveOvitoButton;
    QPushButton *toggleViewButton;

    QCheckBox *gridToggle;
    QCheckBox *ereaseToggle;

    QSlider *layerSlider;

    QComboBox *neighbourhoodCombo;
    QComboBox *cellSizeCombo;
    QComboBox *ereaseGrainCombo;

    QSpinBox *randomCountSpin;
    QSpinBox *regularStrideSpin;
    QSpinBox *layersCountSpin;

    // Stats
    QLineSeries *meanSizeSeries;
    QChart *meanSizeChart;
    QChartView *meanSizeChartView;
    QValueAxis *meanIterAxis;
    QValueAxis *meanSizeAxis;

    QBarSeries *histSeries;
    QChart *histChart;
    QChartView *histChartView;
    QBarCategoryAxis *histCategoryAxis;
    QValueAxis *histValueAxis;

    void setupUI();
    void setupLayout();
    void setupConnections();
    void updateIterationLabel();
    void setupStats();
    void updateStats();
    void clearStats();

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
    void onToggleView(bool checked);
    void refreshEreaseCombo();
};
