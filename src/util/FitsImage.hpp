#pragma once

#include "Config.hpp"

class FitsImage
{
  public:
    FitsImage(size_t nx, size_t ny, size_t nz = 1);

    void save(const string& filepath) const;

    double& operator()(size_t ix, size_t iy, size_t iz = 0)
    {
        return _data[ix + _nx * (iy + _ny * iz)];
    }

    const double& operator()(size_t ix, size_t iy, size_t iz = 0) const
    {
        return _data[ix + _nx * (iy + _ny * iz)];
    }

  private:
    size_t         _nx, _ny, _nz;
    vector<double> _data;
};