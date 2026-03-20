#include "WavGrid.hpp"
#include <stdexcept>

WavGrid::WavGrid(const vector<real>& bw)
    : wmin(bw[0]),
      wmax(bw[bw.size() - 1]),
      numBins(bw.size() - 1)
{
    this->bw = bw;
    if (wmin > wmax)
        throw std::runtime_error("WavGrid is not strictly increasing");
}