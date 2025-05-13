#pragma once
#include <GridWidget.h>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtWidgets/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onStartSimulation();
    void onResetSimulation();
    void keyPressEvent(QKeyEvent *event);

  private:
    void setupUI();
    void setupLayout();
    void setupConnections();

    GridWidget *grid = nullptr;
    QPushButton *startButton = nullptr;
    QPushButton *resetButton = nullptr;
    QCheckBox *gridToggle = nullptr;
};
