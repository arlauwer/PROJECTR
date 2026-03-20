#pragma once

#include "Config.hpp"
#include <algorithm>

struct WavGrid
{
    WavGrid(const vector<real>& bw);

    real   wmin, wmax;
    size_t numBins;

    // borders (N+1)
    vector<real> bw;

    size_t index(real wav) const
    {
        return std::upper_bound(bw.begin(), bw.end(), wav) - bw.begin();
    }
};