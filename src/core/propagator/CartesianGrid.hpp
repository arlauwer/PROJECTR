#pragma once

#include "Config.hpp"

struct CartesianGrid
{
  public:
    CartesianGrid(real xmin, real xmax, real ymin, real ymax, real zmin, real zmax, size_t Nx, size_t Ny, size_t Nz);

    real   xmin, xmax, ymin, ymax, zmin, zmax;
    size_t Nx, Ny, Nz;

    // borders (N+1)
    vector<real> bx;
    vector<real> by;
    vector<real> bz;
};