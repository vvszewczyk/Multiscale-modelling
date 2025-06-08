#include "MainWindow.h"
#include "Constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gridWidget(new GridWidget(this)),
      sim(new Simulation(Config::gridCols, Config::gridRows, Config::gridDepth)), timer(new QTimer(this)),
      simulationLabel(new QLabel("Simulation", this)), startButton(new QPushButton("Start", this)),
      resetButton(new QPushButton("Reset", this)), gridToggle(new QCheckBox("Show grid", this)),
      ereaseToggle(new QCheckBox("Erease mode", this)), iterationLabel(new QLabel("Iteration: 0", this)),
      layerSlider(new QSlider(Qt::Horizontal, this)), layerLabel(new QLabel("Layer: 0", this)),
      grainsLabel(new QLabel("Place grains", this)), randomGrainsButton(new QPushButton("Random", this)),
      regularGrainsButton(new QPushButton("Regular", this)), neighbourhoodCombo(new QComboBox(this)),
      neighbourhoodLabel(new QLabel("Neighbourhood", this)), cellSizeLabel(new QLabel("Cell size:", this)),
      cellSizeCombo(new QComboBox(this)), randomCountSpin(new QSpinBox(this)), regularStrideSpin(new QSpinBox(this)),
      layersCountLabel(new QLabel("Layers: ", this)), layersCountSpin(new QSpinBox(this)),
      ereaseGrainCombo(new QComboBox(this)), ereaseGrainButton(new QPushButton("Erease grain", this)),
      savePNGButton(new QPushButton("Save PNG", this)), saveOvitoButton(new QPushButton("Save ovito", this))
{
    setFixedSize(Config::windowWidth, Config::windowHeight);
    setupUI();
    setupLayout();
    setupConnections();
}

void MainWindow::setupUI()
{
    this->gridWidget->setFixedSize(Config::gridPixelWidth, Config::gridPixelHeight);
    this->gridWidget->setSimulation(sim);

    this->gridToggle->setChecked(true);
    this->ereaseToggle->setChecked(false);

    // this->sim->seedRandom(Config::randomGrainNumber);
    // this->sim->seedRegular(Config::regularGrainStride, Config::regularGrainStride, Config::regularGrainStride);

    this->layerSlider->setMinimum(0);
    this->layerSlider->setMaximum(Config::gridDepth - 1);
    this->layerSlider->setValue(0);
    layersCountSpin->setRange(1, 200);
    layersCountSpin->setValue(Config::gridDepth);

    cellSizeCombo->addItem("1", QVariant(1));
    cellSizeCombo->addItem("2", QVariant(2));
    cellSizeCombo->addItem("5", QVariant(5));
    cellSizeCombo->addItem("10", QVariant(10));
    cellSizeCombo->setCurrentIndex(3);

    randomCountSpin->setRange(0, 1000000); // akceptujemy dowolną liczbę ziaren do miliona
    randomCountSpin->setValue(Config::randomGrainNumber);

    regularStrideSpin->setRange(1, std::max({Config::gridCols, Config::gridRows, Config::gridDepth}));
    regularStrideSpin->setValue(Config::regularGrainStride);

    neighbourhoodCombo->addItem("Von Neumann");
    neighbourhoodCombo->addItem("Moore");
    neighbourhoodCombo->addItem("Hexagonal Random");
    neighbourhoodCombo->setCurrentIndex(0);

    ereaseGrainCombo->addItem("(none)", QVariant(0));
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

    rightLayout->addWidget(simulationLabel);

    // Start/pause and reset buttons
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addStretch();
    btnRow->addWidget(startButton);
    btnRow->addSpacing(5); // Space between buttons
    btnRow->addWidget(resetButton);
    btnRow->addStretch();
    rightLayout->addLayout(btnRow);

    // Random/regular grain place buttons
    rightLayout->addWidget(grainsLabel);
    QHBoxLayout *grnPlcRow = new QHBoxLayout();
    grnPlcRow->addStretch();
    grnPlcRow->addWidget(randomGrainsButton);
    grnPlcRow->addSpacing(10); // Space between buttons
    grnPlcRow->addWidget(regularGrainsButton);
    grnPlcRow->addStretch();
    rightLayout->addLayout(grnPlcRow);

    QHBoxLayout *spinRow = new QHBoxLayout();
    spinRow->addWidget(randomCountSpin);
    spinRow->addWidget(regularStrideSpin);
    rightLayout->addLayout(spinRow);

    // Neighbourhood combobox
    rightLayout->addWidget(neighbourhoodLabel);
    rightLayout->addWidget(neighbourhoodCombo);

    // Checkboxes
    QHBoxLayout *chckbxRow = new QHBoxLayout();
    chckbxRow->addStretch();
    chckbxRow->addWidget(gridToggle);
    chckbxRow->addWidget(ereaseToggle);
    chckbxRow->addStretch(); // Push checkboxes up
    rightLayout->addLayout(chckbxRow);

    QHBoxLayout *ereaseRow = new QHBoxLayout();
    ereaseRow->addWidget(ereaseGrainCombo);
    ereaseRow->addWidget(ereaseGrainButton);
    rightLayout->addLayout(ereaseRow);

    rightLayout->addWidget(cellSizeLabel);
    rightLayout->addWidget(cellSizeCombo);

    // Depth slider
    rightLayout->addWidget(layerLabel, 0, Qt::AlignCenter);
    rightLayout->addWidget(layerSlider);

    QHBoxLayout *layerRow = new QHBoxLayout();
    layerRow->addWidget(layersCountLabel);
    layerRow->addWidget(layersCountSpin);
    rightLayout->addLayout(layerRow);
    rightLayout->addStretch();

    // Save buttons
    QHBoxLayout *saveRow = new QHBoxLayout();
    saveRow->addWidget(savePNGButton);
    saveRow->addWidget(saveOvitoButton);
    rightLayout->addLayout(saveRow);

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
    connect(timer, &QTimer::timeout, this, &MainWindow::onStep);                     // Start the whole simulation
    connect(layerSlider, &QSlider::valueChanged, this, &MainWindow::onLayerChanged); // Layer slider
    connect(randomGrainsButton, &QPushButton::clicked, this,
            &MainWindow::onRandomGrainsClicked); // Place grains randolmy
    connect(regularGrainsButton, &QPushButton::clicked, this,
            &MainWindow::onRegularGrainsClicked);                                          // Place grains regularly
    connect(neighbourhoodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, // Neighbourhood combobox
            &MainWindow::onNeighbourhoodChanged);
    connect(cellSizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::onCellSizeChanged); // Cell size combo box
    connect(randomCountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MainWindow::onRandomCountChanged); // Random spin
    connect(regularStrideSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MainWindow::onRegularStrideChanged); // Regular spin
    connect(layersCountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onLayersCountChanged);
    connect(ereaseGrainButton, &QPushButton::clicked, this, &MainWindow::onEreaseGrainButtonClicked);
    connect(timer, &QTimer::timeout, this, &MainWindow::refreshEreaseCombo);
    connect(gridWidget, &GridWidget::cellRemoved, this, &MainWindow::refreshEreaseCombo);
    connect(savePNGButton, &QPushButton::clicked, this, &MainWindow::onSavePNGClicked);
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
    gridWidget->update();
    refreshEreaseCombo();
}

void MainWindow::onRandomGrainsClicked()
{
    sim->reset();
    sim->seedRandom(Config::randomGrainNumber);
    gridWidget->update();
}

void MainWindow::onRegularGrainsClicked()
{
    sim->reset();
    sim->seedRegular(Config::regularGrainStride, Config::regularGrainStride, Config::regularGrainStride);
    gridWidget->update();
}

void MainWindow::onNeighbourhoodChanged(int index)
{
    NeighbourhoodType nt = NeighbourhoodType::VonNeumann;

    if (index == 0)
    {
        nt = NeighbourhoodType::VonNeumann;
    }
    else if (index == 1)
    {
        nt = NeighbourhoodType::Moore;
    }
    else if (index == 2)
    {
        nt = NeighbourhoodType::HexagonalRandom;
    }
    sim->setNeighbourhoodType(nt);
}

void MainWindow::onCellSizeChanged(int index)
{
    int newSize = cellSizeCombo->currentData().toInt();
    Config::cellSize = newSize;

    Config::gridCols = std::floor(Config::gridPixelWidth / double(Config::cellSize));
    Config::gridRows = std::floor(Config::gridPixelHeight / double(Config::cellSize));

    delete sim;
    sim = new Simulation(Config::gridCols, Config::gridRows, Config::gridDepth);
    gridWidget->setSimulation(sim);
    gridWidget->setFixedSize(Config::gridPixelWidth, Config::gridPixelHeight);
    gridWidget->update();
    iterationLabel->setText("Iteration: 0");
}

void MainWindow::onRandomCountChanged(int newVal)
{
    Config::randomGrainNumber = newVal;
}

void MainWindow::onRegularStrideChanged(int newVal)
{
    Config::regularGrainStride = newVal;
}

void MainWindow::onLayersCountChanged(int newDepth)
{
    Config::gridDepth = newDepth;
    delete sim;

    sim = new Simulation(Config::gridCols, Config::gridRows, Config::gridDepth);
    gridWidget->setSimulation(sim);

    layerSlider->setMaximum(Config::gridDepth - 1);
    layerSlider->setValue(0);
    layerLabel->setText("Layer: 0");

    iterationLabel->setText("Iteration: 0");
    gridWidget->update();
}

void MainWindow::onEreaseGrainButtonClicked()
{
    int id = ereaseGrainCombo->currentData().toInt();

    if (id <= 0)
    {
        return;
    }

    sim->removeGrainByID(id);
    gridWidget->update();
    refreshEreaseCombo();
}

void MainWindow::refreshEreaseCombo()
{
    const Grid &g = sim->getGrid();
    int W = g.getCols();
    int H = g.getRows();
    int z = layerSlider->value();
    QMap<int, QColor> info;
    for (int x = 0; x < W; ++x)
    {
        for (int y = 0; y < H; ++y)
        {
            if (g.at(x, y, z).getState() == State::Occupied)
            {
                int id = g.at(x, y, z).getGrainID();
                if (!info.contains(id))
                {
                    info[id] = g.at(x, y, z).colorForState();
                }
            }
        }
    }

    int prev = ereaseGrainCombo->currentData().toInt();
    ereaseGrainCombo->clear();
    ereaseGrainCombo->addItem("(none)", QVariant(0));

    for (auto it = info.constBegin(); it != info.constEnd(); ++it)
    {
        int id = it.key();
        QColor col = it.value();

        QPixmap pix(12, 12);
        pix.fill(col);
        QIcon icon(pix);

        ereaseGrainCombo->addItem(icon, QString::number(id), QVariant(id));
    }
    int idx = ereaseGrainCombo->findData(QVariant(prev));
    ereaseGrainCombo->setCurrentIndex(idx >= 0 ? idx : 0);
}

void MainWindow::onSavePNGClicked()
{
    QString fn = QFileDialog::getSaveFileName(
        this, tr("Save snapshot as..."), QString("snapshot_%1.png").arg(sim->getIteration()), tr("PNG Files (*.png)"));

    if (fn.isEmpty())
    {
        return;
    }

    QPixmap pix = gridWidget->grab();

    if (!pix.save(fn))
    {
        QMessageBox::warning(this, tr("Save failed"), tr("Could not save snapshot to:\n%1").arg(fn));
    }
    else
    {
        QMessageBox::information(this, tr("Saved"), tr("Snapshot saved to:\n%1").arg(fn));
    }
}

MainWindow::~MainWindow()
{
    delete sim;
}
