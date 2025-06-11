#include "MainWindow.h"
#include "Constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gridWidget(new GridWidget(this)),
      sim(new Simulation(Config::gridCols, Config::gridRows, Config::gridDepth)), timer(new QTimer(this)),
      simulationLabel(new QLabel("Simulation settings", this)), startButton(new QPushButton("Start", this)),
      resetButton(new QPushButton("Reset", this)), gridToggle(new QCheckBox("Show grid", this)),
      ereaseToggle(new QCheckBox("Erease mode", this)), iterationLabel(new QLabel("Iteration: 0", this)),
      layerSlider(new QSlider(Qt::Horizontal, this)), layerLabel(new QLabel("Layer: 0", this)),
      grainsLabel(new QLabel("Place grains", this)), randomGrainsButton(new QPushButton("Random", this)),
      regularGrainsButton(new QPushButton("Regular", this)), neighbourhoodCombo(new QComboBox(this)),
      neighbourhoodLabel(new QLabel("Neighbourhood", this)), cellSizeLabel(new QLabel("Cell size:", this)),
      cellSizeCombo(new QComboBox(this)), randomCountSpin(new QSpinBox(this)), regularStrideSpin(new QSpinBox(this)),
      layersCountLabel(new QLabel("Layers: ", this)), layersCountSpin(new QSpinBox(this)),
      ereaseGrainCombo(new QComboBox(this)), ereaseGrainButton(new QPushButton("Erease grain", this)),
      savePNGButton(new QPushButton("Save PNG", this)), saveOvitoButton(new QPushButton("Save ovito", this)),
      statsWidget(new QWidget(this)), contentStack(new QStackedWidget(centralWidget())),
      toggleViewButton(new QPushButton("Show stats", this)), caDone(false),
      monteCarloLabel(new QLabel("Monte Carlo settings", this)), mcTotal(0), mcDone(0),
      mcRemainingLabel(new QLabel("MC left: 0", this)), mcStepsSpin(new QSpinBox(this)),
      jgbLabel(new QLabel("Jgb:", this)), jgbSpin(new QDoubleSpinBox(this)), ktLabel(new QLabel("kt:", this)),
      ktSpin(new QDoubleSpinBox(this))
{
    setFixedSize(Config::windowWidth, Config::windowHeight);
    setupUI();
    setupStats();
    setupLayout();
    setupConnections();
}

void MainWindow::setupUI()
{
    this->gridWidget->setFixedSize(Config::gridPixelWidth, Config::gridPixelHeight);
    this->gridWidget->setSimulation(sim);

    this->gridToggle->setChecked(true);
    this->ereaseToggle->setChecked(false);

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

    randomCountSpin->setRange(0, 1000000);
    randomCountSpin->setValue(Config::randomGrainNumber);

    regularStrideSpin->setRange(1, std::max({Config::gridCols, Config::gridRows, Config::gridDepth}));
    regularStrideSpin->setValue(Config::regularGrainStride);

    neighbourhoodCombo->addItem("Von Neumann");
    neighbourhoodCombo->addItem("Moore");
    neighbourhoodCombo->addItem("Hexagonal Random");
    neighbourhoodCombo->setCurrentIndex(0);

    ereaseGrainCombo->addItem("(none)", QVariant(0));

    toggleViewButton->setCheckable(true);
    toggleViewButton->setChecked(false);

    mcStepsSpin->setRange(0, 100000);
    mcStepsSpin->setValue(10);

    jgbSpin->setRange(0.0, 5.0);
    jgbSpin->setSingleStep(0.1);
    jgbSpin->setValue(1.0);

    ktSpin->setRange(0.1, 6.0);
    ktSpin->setSingleStep(0.1);
    ktSpin->setValue(0.1);
}

void MainWindow::setupLayout()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    // centralWidget->setStyleSheet(Config::centralWidgetColor); // lightgreen

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *left = new QWidget(centralWidget);
    left->setStyleSheet(Config::leftBlockColor);
    QVBoxLayout *leftLayout = new QVBoxLayout(left);

    contentStack->addWidget(gridWidget);
    contentStack->addWidget(statsWidget);
    contentStack->setCurrentIndex(0);

    leftLayout->addWidget(contentStack, 1);
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

    // Erease combo
    QHBoxLayout *ereaseRow = new QHBoxLayout();
    ereaseRow->addWidget(ereaseGrainCombo);
    ereaseRow->addWidget(ereaseGrainButton);
    rightLayout->addLayout(ereaseRow);

    // Cell size
    rightLayout->addWidget(cellSizeLabel);
    rightLayout->addWidget(cellSizeCombo);

    // Depth slider
    rightLayout->addWidget(layerLabel, 0, Qt::AlignCenter);
    rightLayout->addWidget(layerSlider);

    // Layer number
    QHBoxLayout *layerRow = new QHBoxLayout();
    layerRow->addWidget(layersCountLabel);
    layerRow->addWidget(layersCountSpin);
    rightLayout->addLayout(layerRow);

    // Monte Carlo steps
    rightLayout->addWidget(monteCarloLabel);
    QHBoxLayout *mcRow = new QHBoxLayout();
    mcRow->addWidget(mcStepsSpin);
    mcRow->addWidget(mcRemainingLabel);
    rightLayout->addLayout(mcRow);

    // Monte carlo constants
    QHBoxLayout *jgbRow = new QHBoxLayout();
    jgbRow->addWidget(jgbLabel);
    jgbRow->addWidget(jgbSpin);
    rightLayout->addLayout(jgbRow);

    QHBoxLayout *ktRow = new QHBoxLayout();
    ktRow->addWidget(ktLabel);
    ktRow->addWidget(ktSpin);
    rightLayout->addLayout(ktRow);

    rightLayout->addStretch();

    // Grid/stats button
    rightLayout->addWidget(toggleViewButton, 0, Qt::AlignCenter);

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

void MainWindow::setupStats()
{
    QVBoxLayout *statsL = new QVBoxLayout(statsWidget);

    QGroupBox *summaryBox = new QGroupBox("Summary", statsWidget);
    QFormLayout *form = new QFormLayout(summaryBox);
    totalGrainsLabel = new QLabel("0", summaryBox);
    occupiedFractionLabel = new QLabel("0%", summaryBox);
    avgSizeLabel = new QLabel("0", summaryBox);
    stdDevLabel = new QLabel("0", summaryBox);
    iterationValueLabel = new QLabel("0", summaryBox);
    form->addRow("Number of grains:", totalGrainsLabel);
    form->addRow("Filling [%]:", occupiedFractionLabel);
    form->addRow("AVG size:", avgSizeLabel);
    form->addRow("Standard Deviation:", stdDevLabel);
    form->addRow("Iteration:", iterationValueLabel);
    statsL->addWidget(summaryBox);

    meanSizeSeries = new QLineSeries(this);
    meanSizeChart = new QChart();
    meanSizeChart->addSeries(meanSizeSeries);
    meanSizeChart->legend()->hide();
    meanSizeChart->setTitle("AVG grain size");

    meanIterAxis = new QValueAxis();
    meanIterAxis->setTitleText("Iteration");
    meanIterAxis->setLabelFormat("%d");
    meanSizeChart->addAxis(meanIterAxis, Qt::AlignBottom);
    meanSizeSeries->attachAxis(meanIterAxis);

    meanSizeAxis = new QValueAxis();
    meanSizeAxis->setTitleText("AVG size");
    meanSizeAxis->setLabelFormat("%.1f");
    meanSizeChart->addAxis(meanSizeAxis, Qt::AlignLeft);
    meanSizeSeries->attachAxis(meanSizeAxis);

    meanSizeChartView = new QChartView(meanSizeChart, statsWidget);
    meanSizeChartView->setRenderHint(QPainter::Antialiasing);

    statsL->addWidget(meanSizeChartView, 1);

    histSeries = new QBarSeries(this);
    histChart = new QChart();
    histChart->addSeries(histSeries);
    histChart->setTitle("Grain size histogram");
    histChart->legend()->setVisible(false);

    histCategoryAxis = new QBarCategoryAxis();
    histChart->addAxis(histCategoryAxis, Qt::AlignBottom);
    histSeries->attachAxis(histCategoryAxis);

    histValueAxis = new QValueAxis();
    histChart->addAxis(histValueAxis, Qt::AlignLeft);
    histSeries->attachAxis(histValueAxis);

    histChartView = new QChartView(histChart, statsWidget);
    histChartView->setRenderHint(QPainter::Antialiasing);
    statsL->addWidget(histChartView, 1);

    statsL->setStretchFactor(meanSizeChartView, 1);
    statsL->setStretchFactor(histChartView, 1);
}

void MainWindow::setupConnections()
{
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);          // Start button event
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);          // Reset button event
    connect(gridToggle, &QCheckBox::stateChanged, gridWidget, &GridWidget::setShowGrid);     // Grid toggle
    connect(ereaseToggle, &QCheckBox::stateChanged, gridWidget, &GridWidget::setEreaseMode); // Erease mode toggle
    connect(toggleViewButton, &QPushButton::toggled, this, &MainWindow::onToggleView);
    connect(timer, &QTimer::timeout, this, [&]() {
        onStep();
        int globalStep = sim->getIteration() + mcDone;
        updateStats(globalStep);
    }); // Start simulation and update statistics
    connect(layerSlider, &QSlider::valueChanged, this, &MainWindow::onLayerChanged); // Layer slider
    connect(randomGrainsButton, &QPushButton::clicked, this,
            &MainWindow::onRandomGrainsClicked); // Place grains randolmy
    connect(regularGrainsButton, &QPushButton::clicked, this,
            &MainWindow::onRegularGrainsClicked); // Place grains regularly
    connect(neighbourhoodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::onNeighbourhoodChanged); // Neighbourhood combobox
    connect(cellSizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::onCellSizeChanged); // Cell size combo box
    connect(randomCountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MainWindow::onRandomCountChanged); // Random spin
    connect(regularStrideSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MainWindow::onRegularStrideChanged); // Regular spin
    connect(layersCountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MainWindow::onLayersCountChanged); // Layers spin
    connect(ereaseGrainButton, &QPushButton::clicked, this,
            &MainWindow::onEreaseGrainButtonClicked);                        // Erease grain (button for combo)
    connect(timer, &QTimer::timeout, this, &MainWindow::refreshEreaseCombo); // Refreshing erease combobox
    connect(gridWidget, &GridWidget::cellRemoved, this, &MainWindow::refreshEreaseCombo); // Removing grains with combo
    connect(savePNGButton, &QPushButton::clicked, this, &MainWindow::onSavePNGClicked);   // Save as PNG
    connect(saveOvitoButton, &QPushButton::clicked, this, &MainWindow::onSaveOvitoClicked); // Save as Ovito
    connect(mcStepsSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int v) {
        mcTotal = v;
        mcDone = 0;
        mcRemainingLabel->setText(QString("MC left: %1").arg(mcTotal));
    }); // Monte carlo steps
    connect(jgbSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [&](double v) { sim->setJgb(v); }); // Jgb spin
    connect(ktSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [&](double v) { sim->setKt(v); }); // kt spin
}

// Widget methods

void MainWindow::onStartClicked()
{
    if (timer->isActive())
    {
        timer->stop();
        startButton->setText("Start");
    }
    else
    {
        mcRemainingLabel->setText(QString("MC left: %1").arg(mcTotal));
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
    clearStats();
    updateIterationLabel(0);
    caDone = false;
    mcTotal = 0;
}

void MainWindow::onStep()
{
    if (!caDone)
    {
        // CA
        sim->step();
        if (sim->getGrid().countEmpty() == 0)
        {
            caDone = true;
        }
    }
    else if (mcDone < mcTotal)
    {
        // MC
        sim->mcStep();
        ++mcDone;
        mcRemainingLabel->setText(QString("MC left: %1").arg(mcTotal - mcDone));
    }
    else
    {
        timer->stop();
        startButton->setText("Start");
    }

    gridWidget->update();

    int globalStep = sim->getIteration() + mcDone;
    updateIterationLabel(globalStep);

    if (contentStack->currentIndex() == 1)
    {
        updateStats(globalStep);
    }
}

void MainWindow::updateIterationLabel(int globalStep)
{
    iterationLabel->setText(QString("Iteration: %1").arg(globalStep));
    meanIterAxis->setRange(0, std::max(globalStep, 10));
}

void MainWindow::onLayerChanged(int newZ)
{
    layerLabel->setText(QString("Layer: %1").arg(newZ));
    gridWidget->setLayer(newZ);
    gridWidget->update();
    refreshEreaseCombo();
    int globalStep = sim->getIteration() + mcDone;

    if (contentStack->currentIndex() == 1)
    {
        updateStats(globalStep);
    }
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

void MainWindow::onSaveOvitoClicked()
{
    QString fn =
        QFileDialog::getSaveFileName(this, tr("Save for Ovito..."), "structure.txt", tr("Text files (*.txt *.xyz)"));
    if (fn.isEmpty())
        return;

    const Grid &g = sim->getGrid();
    int W = g.getCols(), H = g.getRows(), D = g.getDepth();

    struct Item
    {
        int id;
        int x, y, z;

        Item(int id, int x, int y, int z) : id(id), x(x), y(y), z(z) {};
    };
    std::vector<Item> items;
    items.reserve(W * H * D);

    for (int z = 0; z < D; ++z)
    {
        for (int y = 0; y < H; ++y)
        {
            for (int x = 0; x < W; ++x)
            {
                const Cell &c = g.at(x, y, z);
                int id = c.getState() == State::Occupied ? c.getGrainID() : 0;
                if (id == 0)
                    continue;
                items.push_back({id, x * Config::cellSize, y * Config::cellSize, z * Config::cellSize});
            }
        }
    }

    std::ofstream out(fn.toStdString());
    if (!out)
    {
        QMessageBox::warning(this, "Error", "Cannot open file for writing");
        return;
    }

    out << items.size() << "\n";
    out << "# CA_iter=" << sim->getIteration() << "  MC_done=" << mcDone << "/" << mcTotal << "\n";

    for (auto &it : items)
    {
        out << it.id << " " << it.x << " " << it.y << " " << it.z << "\n";
    }
    out.close();

    QMessageBox::information(this, "Saved", QString("Ovito file written to:\n%1").arg(fn));
}

void MainWindow::onToggleView(bool checked)
{
    int globalStep = sim->getIteration() + mcDone;
    if (checked)
    {
        contentStack->setCurrentIndex(1);
        toggleViewButton->setText("Show simulation");
        updateStats(globalStep);
    }
    else
    {
        contentStack->setCurrentIndex(0);
        toggleViewButton->setText("Show stats");
    }
}

void MainWindow::updateStats(int globalStep)
{
    const Grid &g = sim->getGrid();
    int W = g.getCols(), H = g.getRows(), D = g.getDepth();
    int z = layerSlider->value(); // or 0

    std::map<int, int> grainCounts;
    int occupied = 0;
    for (int x = 0; x < W; ++x)
    {
        for (int y = 0; y < H; ++y)
        {
            const Cell &c = g.at(x, y, z);
            if (c.getState() == State::Occupied)
            {
                ++occupied;
                ++grainCounts[c.getGrainID()];
            }
        }
    }
    int totalCells = W * H;
    int totalGrains = grainCounts.size();

    // AVG size and stddev
    std::vector<double> sizes;
    sizes.reserve(totalGrains);
    for (auto &p : grainCounts)
    {
        sizes.push_back(p.second);
    }

    double sum = std::accumulate(sizes.begin(), sizes.end(), 0.0);
    double mean = sizes.empty() ? 0.0 : sum / sizes.size();
    double var = 0;
    for (double s : sizes)
    {
        var += (s - mean) * (s - mean);
    }
    double stddev = sizes.empty() ? 0.0 : std::sqrt(var / sizes.size());

    // Set texts
    totalGrainsLabel->setText(QString::number(totalGrains));
    occupiedFractionLabel->setText(QString::number(100.0 * occupied / totalCells, 'f', 1) + "%");
    avgSizeLabel->setText(QString::number(mean, 'f', 2));
    stdDevLabel->setText(QString::number(stddev, 'f', 2));
    iterationValueLabel->setText(QString::number(globalStep));

    // Add point to AVG size plot
    meanSizeSeries->append(globalStep, mean);
    meanIterAxis->setRange(0, std::max(globalStep, 10));
    meanSizeAxis->setRange(0, std::max(5.0, mean * 1.2));

    const int binCount = 10;
    int maxSize = sizes.empty() ? 0 : int(*std::max_element(sizes.begin(), sizes.end()));
    int bins = std::max(1, std::min(binCount, maxSize));
    std::vector<int> binCounts(bins, 0);
    double binWidth = double(maxSize) / bins;

    for (auto &p : grainCounts)
    {
        int size = p.second;
        int idx = std::min(bins - 1, int((size - 1) / binWidth));
        binCounts[idx] += p.second;
    }

    QStringList categories;
    for (int i = 0; i < bins; ++i)
    {
        int lo = int(std::round(i * binWidth)) + 1;
        int hi = int(std::round((i + 1) * binWidth));
        categories << QString("%1–%2").arg(lo).arg(hi);
    }

    histSeries->clear();
    QBarSet *set = new QBarSet("Count");
    for (int cnt : binCounts)
    {
        *set << cnt;
    }
    histSeries->append(set);

    histCategoryAxis->clear();
    histCategoryAxis->append(categories);

    histCategoryAxis->setTitleText("Grain size");
    histValueAxis->setTitleText("Count");

    qreal maxVal = *std::max_element(binCounts.begin(), binCounts.end());
    histValueAxis->setRange(0, maxVal + 1);
}

void MainWindow::clearStats()
{
    meanSizeSeries->clear();
    histSeries->clear();
    histCategoryAxis->clear();

    meanIterAxis->setRange(0, 10);
    meanSizeAxis->setRange(0, 5);
    histValueAxis->setRange(0, 1);

    totalGrainsLabel->setText("0");
    occupiedFractionLabel->setText("0%");
    avgSizeLabel->setText("0");
    stdDevLabel->setText("0");
    iterationValueLabel->setText("0");
}

MainWindow::~MainWindow()
{
    delete sim;
}
