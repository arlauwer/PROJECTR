#include "WavGrid.hpp"
#include <stdexcept>

WavGrid::WavGrid(const vector<real>& borders)
    : borders(borders),
      numBins(borders.size() - 1)
{
    if (numBins < 1)
        throw std::runtime_error("WavGrid must have at least 2 borders");

    std::sort(this->borders.begin(), this->borders.end());

    wmin = borders[0];
    wmax = borders[numBins];
}