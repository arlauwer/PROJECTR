#pragma once

#include "Config.hpp"
#include <algorithm>

struct WavGrid
{
    WavGrid(const vector<real>& borders);

    int index(real wav) const
    {
        return std::upper_bound(borders.begin(), borders.end(), wav) - borders.begin() - 1;
    }

    real width(size_t bin) const
    {
        return borders[bin + 1] - borders[bin];
    }

    vector<real> borders;
    size_t       numBins;
    real         wmin, wmax;
};