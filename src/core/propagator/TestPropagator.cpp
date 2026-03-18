#include "TestPropagator.hpp"
#include <cmath>
#include <cstddef>
#include <float.h>

TestPropagator::TestPropagator(real extent, size_t N)
    : _grid(-extent, extent, -extent, extent, -extent, extent, N, N, N)
{
}

void TestPropagator::propagate(Batch& batch)
{
    size_t Nx = _grid.Nx;
    size_t Ny = _grid.Ny;
    size_t Nz = _grid.Nz;
    size_t N  = Nx * Ny * Nz;

    real dx = (_grid.xmax - _grid.xmin) / (real)Nx;
    real dy = (_grid.ymax - _grid.ymin) / (real)Ny;
    real dz = (_grid.zmax - _grid.zmin) / (real)Nz;

    batch.for_each(
        [this, &batch, Nx, Ny, Nz, N, dx, dy, dz](size_t b)
        {
            real& accum  = batch.accum[b];
            real& target = batch.target[b];

            real& rx = batch.rx[b];
            real& ry = batch.ry[b];
            real& rz = batch.rz[b];

            real& nx = batch.nx[b];
            real& ny = batch.ny[b];
            real& nz = batch.nz[b];

            int& m = batch.m[b];
            int& i = batch.i[b];
            int& j = batch.j[b];
            int& k = batch.k[b];

            // set initial cell indices
            i = std::floor((rx - _grid.xmin) / dx);
            j = std::floor((ry - _grid.ymin) / dy);
            k = std::floor((rz - _grid.zmin) / dz);
            m = i * Nz * Ny + j * Ny + k;

            // while inside the grid
            while (0 <= m && m < N)
            {
                int xdir = sign(nx);
                int ydir = sign(ny);
                int zdir = sign(nz);

                real xE  = _grid.bx[i + (xdir + 1) / 2];
                real yE  = _grid.by[j + (ydir + 1) / 2];
                real zE  = _grid.bz[k + (zdir + 1) / 2];
                real dsx = (std::abs(nx) > 1e-15) ? (xE - rx) / nx : REAL_MAX;
                real dsy = (std::abs(ny) > 1e-15) ? (yE - ry) / ny : REAL_MAX;
                real dsz = (std::abs(nz) > 1e-15) ? (zE - rz) / nz : REAL_MAX;

                // choose the smallest distance
                real ds;
                if (dsx < dsy && dsx < dsz)
                    ds = dsx;
                else if (dsy < dsz)
                    ds = dsy;
                else
                    ds = dsz;

                double accum_next = accum + ds;

                if (accum_next > target)
                {
                    ds = target - accum;

                    // propagate to the target
                    rx += nx * ds;
                    ry += ny * ds;
                    rz += nz * ds;
                    accum = target;
                    break; // break at target
                }

                // propagate to the next cell
                rx += nx * ds;
                ry += ny * ds;
                rz += nz * ds;
                accum = accum_next;

                if (dsx <= dsy && dsx <= dsz)
                {
                    i += xdir;
                    m += xdir * Nz * Ny;
                }
                else if (dsy <= dsz)
                {
                    j += ydir;
                    m += ydir * Nz;
                }
                else
                {
                    k += zdir;
                    m += zdir;
                }
            }
        }
    );
}