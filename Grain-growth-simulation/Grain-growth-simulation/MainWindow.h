#pragma once
#include <GridWidget.h>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
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

    void setupUI();
    void setupLayout();
    void setupConnections();
    void updateIterationLabel();

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    void onStartClicked();
    void onResetClicked();
    void onStep();
};
