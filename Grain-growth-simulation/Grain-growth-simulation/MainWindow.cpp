#include "MainWindow.h"
#include "Constants.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(Config::windowWidth, Config::windowHeight);
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
    startButton->setFixedSize(Config::buttonWidth, Config::buttonHeight);
    resetButton->setFixedSize(Config::buttonWidth, Config::buttonHeight);

    gridToggle = new QCheckBox("Show grid", this);
    gridToggle->setChecked(true);
}

void MainWindow::setupLayout()
{
    QWidget *centralWidget = this->centralWidget();
    centralWidget->setStyleSheet(Config::centralWidgetColor); // lightgreen

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left panel (grid)
    QWidget *leftBlock = new QWidget(centralWidget);
    leftBlock->setStyleSheet(Config::leftBlockColor); // lightpink
    QVBoxLayout *leftLayout = new QVBoxLayout(leftBlock);
    leftLayout->setContentsMargins(Config::margin, Config::margin, Config::margin, Config::margin);
    leftLayout->setSpacing(0);
    grid->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    grid->setFixedSize(Config::gridWidth, Config::gridHeight);

    // Grid position: left corner
    leftLayout->addWidget(grid, /*stretch=*/0, Qt::AlignTop | Qt::AlignLeft);

    mainLayout->addWidget(leftBlock, /*stretch=*/7);

    // Right panel (buttons and checkbox)
    QWidget *rightBlock = new QWidget(centralWidget);
    rightBlock->setStyleSheet(Config::rightBlockColor); // lightblue
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
    connect(this->resetButton, &QPushButton::clicked, this, &MainWindow::onResetSimulation);
    connect(this->gridToggle, &QCheckBox::stateChanged, this,
            [this](int state) { grid->setShowGrid(state == Qt::Checked); });
}

void MainWindow::onStartSimulation()
{
    QMessageBox::information(this, "Info", "Simulation has started.");
}

void MainWindow::onResetSimulation()
{
    QMessageBox::information(this, "Info", "Simulation has restarted.");
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
