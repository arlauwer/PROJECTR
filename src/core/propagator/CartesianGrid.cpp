#include "CartesianGrid.hpp"

CartesianGrid::CartesianGrid(
    real xmin, real xmax, real ymin, real ymax, real zmin, real zmax, size_t Nx, size_t Ny, size_t Nz
)
    : xmin(xmin),
      xmax(xmax),
      ymin(ymin),
      ymax(ymax),
      zmin(zmin),
      zmax(zmax),
      Nx(Nx),
      Ny(Ny),
      Nz(Nz)
{
    bx.resize(Nx + 1);
    by.resize(Ny + 1);
    bz.resize(Nz + 1);

    for (size_t i = 0; i < Nx + 1; i++)
    {
        bx[i] = xmin + (xmax - xmin) * i / Nx;
    }

    for (size_t i = 0; i < Ny + 1; i++)
    {
        by[i] = ymin + (ymax - ymin) * i / Ny;
    }

    for (size_t i = 0; i < Nz + 1; i++)
    {
        bz[i] = zmin + (zmax - zmin) * i / Nz;
    }
}