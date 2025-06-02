#include "GridWidget.h"

namespace Config
{
// Grid
const int cellSize = 5; // 1, 2, 5, 10
const int gridCols = std::ceil(860 / double(Config::cellSize));
const int gridRows = std::ceil(790 / double(Config::cellSize));
const int gridDepth = 50;
const int gridWidth = gridCols * cellSize;
const int gridHeight = gridRows * cellSize;

// Main window
const int windowWidth = 1065;
const int windowHeight = 808;

// Buttons
const int buttonWidth = 50;
const int buttonHeight = 20;

// Simulation
const int randomGrainNumber = 500;
const int regularGrainStride = 10;

// Panel colors
const char centralWidgetColor[] = "background-color: #74f740"; // lightgreen
const char leftBlockColor[] = "background-color: #ffe4e1;";    // lightpink
const char rightBlockColor[] = "background-color: #e1eaff;";   // lightblue

// Margin/padding
const int margin = 5;
} // namespace Config
