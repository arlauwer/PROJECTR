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

            uint64_t& m = batch.m[b];
            uint32_t& i = batch.i[b];
            uint32_t& j = batch.j[b];
            uint32_t& k = batch.k[b];

            // set initial cell indices
            i = std::floor((rx - _grid.xmin) / dx);
            j = std::floor((ry - _grid.ymin) / dy);
            k = std::floor((rz - _grid.zmin) / dz);
            m = i * Nz * Ny + j * Ny + k;

            while (accum < target && 0 <= m && m < N)
            {
                // fix this crap
                uint32_t xdir = sign(nx);
                uint32_t ydir = sign(ny);
                uint32_t zdir = sign(nz);

                // real xE  = (xdir > 0) ? _grid.bx[i + 1] : _grid.bx[i];
                // real yE  = (ydir > 0) ? _grid.by[j + 1] : _grid.by[j];
                // real zE  = (zdir > 0) ? _grid.bz[k + 1] : _grid.bz[k];
                real xE  = _grid.bx[i + (xdir + 1) / 2];
                real yE  = _grid.by[j + (ydir + 1) / 2];
                real zE  = _grid.bz[k + (zdir + 1) / 2];
                real dsx = (std::abs(nx) > 1e-15) ? (xE - rx) / nx : REAL_MAX;
                real dsy = (std::abs(ny) > 1e-15) ? (yE - ry) / ny : REAL_MAX;
                real dsz = (std::abs(nz) > 1e-15) ? (zE - rz) / nz : REAL_MAX;

                if (dsx <= dsy && dsx <= dsz)
                {
                    rx = xE;
                    ry += ny * dsx;
                    rz += nz * dsx;
                    i += xdir;
                    m += xdir * Nz * Ny;
                    accum += dsx;
                }
                else if (dsy < dsx && dsy <= dsz)
                {
                    ry = yE;
                    rx += nx * dsy;
                    rz += nz * dsy;
                    j += ydir;
                    m += ydir * Nz;
                    accum += dsy;
                }
                else
                {
                    rz = zE;
                    rx += nx * dsz;
                    ry += ny * dsz;
                    k += zdir;
                    m += zdir;
                    accum += dsz;
                }
            }
        }
    );
}