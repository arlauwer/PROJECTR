#pragma once

#include "Config.hpp"
#include <algorithm>

struct WavGrid
{
    WavGrid(const vector<real>& borders);

    vector<real> borders;
    size_t       numBins;
    real         wmin, wmax;

    size_t index(real wav) const
    {
        return std::upper_bound(borders.begin(), borders.end(), wav) - borders.begin();
    }
};