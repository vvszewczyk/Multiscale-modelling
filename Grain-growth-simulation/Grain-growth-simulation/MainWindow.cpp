#include "MainWindow.h"
#include "GridWidget.h"
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(1000, 800);
    setupUI();
    setupLayout();
    setupConnections();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    this->grid = new GridWidget(this);

    startButton = new QPushButton("Start", this);
    resetButton = new QPushButton("Reset", this);
    startButton->setFixedSize(50, 20);
    resetButton->setFixedSize(50, 20);

    gridToggle = new QCheckBox("Show grid", this);
    gridToggle->setChecked(true);
}

void MainWindow::setupLayout()
{
    QWidget *cw = this->centralWidget();
    cw->setStyleSheet("background-color: #f0f0f0;");

    auto *mainLayout = new QHBoxLayout(cw);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left panel (grid)
    QWidget *leftBlock = new QWidget(cw);
    leftBlock->setStyleSheet("background-color: #ffe4e1;"); // lightpink
    auto *leftLayout = new QVBoxLayout(leftBlock);
    leftLayout->setContentsMargins(5, 5, 5, 5);
    leftLayout->setSpacing(0);

    constexpr int gridW = 860;
    constexpr int gridH = 790;
    grid->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    grid->setFixedSize(gridW, gridH);

    // Grid position: left corner
    leftLayout->addWidget(grid, /*stretch=*/0, Qt::AlignTop | Qt::AlignLeft);

    mainLayout->addWidget(leftBlock, /*stretch=*/7);

    // Right panel (buttons and checkbox)
    QWidget *rightBlock = new QWidget(cw);
    rightBlock->setStyleSheet("background-color: #e1eaff;"); // lightblue
    auto *rightLayout = new QVBoxLayout(rightBlock);
    rightLayout->setContentsMargins(5, 5, 5, 5);
    rightLayout->setSpacing(10);

    // Start/Reset
    auto *btnRow = new QHBoxLayout;
    btnRow->addStretch();
    btnRow->addWidget(startButton);
    btnRow->addSpacing(10);
    btnRow->addWidget(resetButton);
    btnRow->addStretch();
    rightLayout->addLayout(btnRow);

    // checkbox
    rightLayout->addWidget(gridToggle, /*stretch=*/0, Qt::AlignHCenter);
    rightLayout->addStretch();
    mainLayout->addWidget(rightBlock, /*stretch=*/1);
}

void MainWindow::setupConnections()
{
    connect(this->startButton, &QPushButton::clicked, this, &MainWindow::onStartSimulation);
    connect(this->exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(this->gridToggle, &QCheckBox::stateChanged, this,
            [this](int state) { grid->setShowGrid(state == Qt::Checked); });
}

void MainWindow::onStartSimulation()
{
    QMessageBox::information(this, "Info", "Simulation has started.");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
}

MainWindow::~MainWindow()
{
}
