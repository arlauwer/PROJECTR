#pragma once

#include "Config.hpp"
#include <atomic>

struct RadiationField
{
    RadiationField(size_t numCells, size_t numBins)
        : _numCells(numCells),
          _numBins(numBins),
          _field(numCells * numBins)
    {
        std::fill(_field.begin(), _field.end(), 0.);
    }

    size_t flatIndex(size_t cell, size_t bin) const
    {
        return cell * _numBins + bin;
    }

    std::atomic<real>& operator()(size_t cell, size_t bin)
    {
        return _field[flatIndex(cell, bin)];
    }

  private:
    size_t _numCells;
    size_t _numBins;

    std::vector<std::atomic<real>> _field;
};