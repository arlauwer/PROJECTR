#pragma once

#include "Config.hpp"
#include "Math.hpp"
#include "WavGrid.hpp"
#include <atomic>

struct RadiationField
{
    RadiationField(size_t numCells, const vector<real>& borders)
        : numBins(borders.size() - 1),
          wavGrid(borders),
          _field(numCells * numBins)
    {
        Math::fill(_field, 0.);
    }

    size_t flatIndex(size_t cell, size_t bin) const
    {
        return cell * numBins + bin;
    }

    std::atomic<real>& operator()(size_t cell, size_t bin)
    {
        return _field[flatIndex(cell, bin)];
    }

    const std::atomic<real>& operator()(size_t cell, size_t bin) const
    {
        return _field[flatIndex(cell, bin)];
    }

    const size_t  numBins;
    const WavGrid wavGrid;

  private:
    std::vector<std::atomic<real>> _field;
};