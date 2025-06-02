#include "MainWindow.h"
#include "Constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gridWidget(new GridWidget(this)),
      sim(new Simulation(Config::gridCols, Config::gridRows, Config::gridDepth)), timer(new QTimer(this)),
      startButton(new QPushButton("Start", this)), resetButton(new QPushButton("Reset", this)),
      gridToggle(new QCheckBox("Show grid", this)), ereaseToggle(new QCheckBox("Erease mode", this)),
      iterationLabel(new QLabel("Iteration: 0", this)), layerSlider(new QSlider(Qt::Horizontal, this)),
      layerLabel(new QLabel("Layer: 0", this)), placeGrains(new QPushButton("Place grains", this)),
      neighbourhood(new QComboBox(this))
{
    setFixedSize(Config::windowWidth, Config::windowHeight);
    setupUI();
    setupLayout();
    setupConnections();
}

void MainWindow::setupUI()
{
    this->gridWidget->setFixedSize(Config::gridWidth, Config::gridHeight);
    this->gridWidget->setSimulation(sim);

    this->gridToggle->setChecked(true);
    this->ereaseToggle->setChecked(false);

    this->sim->seedRandom(Config::grainNumber);

    this->layerSlider->setMinimum(0);
    this->layerSlider->setMaximum(Config::gridDepth - 1);
    this->layerSlider->setValue(0);

    neighbourhood->addItem("Von Neumann");
    neighbourhood->addItem("Moore");
    neighbourhood->setCurrentIndex(0);
}

void MainWindow::setupLayout()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    // centralWidget->setStyleSheet(Config::centralWidgetColor); // lightgreen

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *left = new QWidget(centralWidget);
    // left->setStyleSheet(Config::leftBlockColor);
    QVBoxLayout *leftLayout = new QVBoxLayout(left);
    leftLayout->addWidget(gridWidget, 0, Qt::AlignTop | Qt::AlignLeft);
    leftLayout->addStretch();
    mainLayout->addWidget(left, 7);

    QWidget *right = new QWidget(centralWidget);
    // right->setStyleSheet(Config::rightBlockColor);
    QVBoxLayout *rightLayout = new QVBoxLayout(right);

    // Start/pause and reset buttons
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addStretch();
    btnRow->addWidget(startButton);
    btnRow->addSpacing(5); // Space between buttons
    btnRow->addWidget(resetButton);
    btnRow->addStretch();
    rightLayout->addLayout(btnRow);

    rightLayout->addWidget(placeGrains);

    rightLayout->addWidget(neighbourhoodLabel);
    rightLayout->addWidget(neighbourhood);

    // Checkboxes
    QHBoxLayout *chckbxRow = new QHBoxLayout();
    chckbxRow->addStretch();
    chckbxRow->addWidget(gridToggle);
    chckbxRow->addWidget(ereaseToggle);
    chckbxRow->addStretch(); // Push checkboxes up
    rightLayout->addLayout(chckbxRow);

    // Depth slider
    rightLayout->addWidget(layerLabel, 0, Qt::AlignCenter);
    rightLayout->addWidget(layerSlider);
    rightLayout->addStretch();

    // Iterations label
    rightLayout->addWidget(iterationLabel, 0, Qt::AlignRight);

    // Add right panel to main layout
    mainLayout->addWidget(right, 1);
}

void MainWindow::setupConnections()
{
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);          // Start button event
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);          // Reset button event
    connect(gridToggle, &QCheckBox::stateChanged, gridWidget, &GridWidget::setShowGrid);     // Grid toggle
    connect(ereaseToggle, &QCheckBox::stateChanged, gridWidget, &GridWidget::setEreaseMode); // Erease mode toggle
    connect(timer, &QTimer::timeout, this, &MainWindow::onStep);                          // Start the whole simulation
    connect(layerSlider, &QSlider::valueChanged, this, &MainWindow::onLayerChanged);      // Layer slider
    connect(placeGrains, &QPushButton::clicked, this, &MainWindow::onPlaceGrainsClicked); // Place grains randolmy
    connect(neighbourhood, QOverload<int>::of(&QComboBox::currentIndexChanged), this,     // Neighbourhood combobox
            &MainWindow::onNeighbourhoodChanged);
}

void MainWindow::onStartClicked()
{
    if (timer->isActive())
    {
        timer->stop();
        startButton->setText("Start");
    }
    else
    {
        timer->start(100);
        startButton->setText("Pause");
    }
}

void MainWindow::onResetClicked()
{
    timer->stop();
    startButton->setText("Start");
    sim->reset();

    // sim->seedRandom(500);

    gridWidget->setLayer(0);
    gridWidget->update();
    updateIterationLabel();
}

void MainWindow::onStep()
{
    sim->step();
    gridWidget->update();
    updateIterationLabel();
}

void MainWindow::updateIterationLabel()
{
    iterationLabel->setText(QString("Iteration: %0").arg(sim->getIteration()));
}

void MainWindow::onLayerChanged(int newZ)
{
    layerLabel->setText(QString("Layer: %1").arg(newZ));
    gridWidget->setLayer(newZ);
}

void MainWindow::onPlaceGrainsClicked()
{
    sim->reset();
    sim->seedRandom(Config::grainNumber);
    gridWidget->update();
}

void MainWindow::onNeighbourhoodChanged(int index)
{
    NeighbourhoodType nt = (index == 0 ? NeighbourhoodType::VonNeumann : NeighbourhoodType::Moore);
    sim->setNeighbourhoodType(nt);
}

MainWindow::~MainWindow()
{
    delete sim;
}
