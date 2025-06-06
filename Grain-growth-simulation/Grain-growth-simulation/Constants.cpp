#include "Constants.h"
#include "GridWidget.h"

namespace Config
{
int cellSize = 10;
int gridCols = std::floor(gridPixelWidth / double(cellSize));
int gridRows = std::floor(gridPixelHeight / double(cellSize));
int gridDepth = 50;
int randomGrainNumber = 500;
int regularGrainStride = 10;
} // namespace Config
