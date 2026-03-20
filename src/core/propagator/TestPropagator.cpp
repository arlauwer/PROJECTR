#include "TestPropagator.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <float.h>

TestPropagator::TestPropagator(real extent, size_t N)
    : _grid(-extent, extent, -extent, extent, -extent, extent, N, N, N),
      _radWavGrid({1.0}),
      _rad(_grid.N, 1) // dangerous order here

{
}

void TestPropagator::propagate(Batch& batch)
{
    const int           Nx    = _grid.Nx;
    const int           Ny    = _grid.Ny;
    const int           Nz    = _grid.Nz;
    const int           N     = _grid.N;
    const int           Nzy   = Nz * Ny;
    const real          dx    = (_grid.xmax - _grid.xmin) / static_cast<real>(Nx);
    const real          dy    = (_grid.ymax - _grid.ymin) / static_cast<real>(Ny);
    const real          dz    = (_grid.zmax - _grid.zmin) / static_cast<real>(Nz);
    const vector<real>& kappa = _grid.kappa;

    batch.for_each(
        [this, &batch, &kappa, Nx, Ny, Nz, N, Nzy, dx, dy, dz](size_t b)
        {
            constexpr real epsilon = 1e-15;

            real&       weight = batch.weight[b];
            const real& lambda = batch.lambda[b];
            real&       accum  = batch.accum[b];
            real&       target = batch.target[b];
            real&       rx     = batch.rx[b];
            real&       ry     = batch.ry[b];
            real&       rz     = batch.rz[b];
            const real& nx     = batch.nx[b];
            const real& ny     = batch.ny[b];
            const real& nz     = batch.nz[b];
            int&        i      = batch.i[b];
            int&        j      = batch.j[b];
            int&        k      = batch.k[b];
            int&        m      = batch.m[b];

            if (m < 0 || m >= _grid.N)
                return;

            // radiation field wavelength grid index
            const size_t radWavIndex = _radWavGrid.index(lambda);

            // Initialize cell indices
            i = static_cast<int>(std::floor((rx - _grid.xmin) / dx));
            j = static_cast<int>(std::floor((ry - _grid.ymin) / dy));
            k = static_cast<int>(std::floor((rz - _grid.zmin) / dz));
            m = i * Nzy + j * Nz + k;

            // Initialize step directions
            const int xdir = sign(nx);
            const int ydir = sign(ny);
            const int zdir = sign(nz);

            // Initialize distance to cross boundary per axis
            const real sdx = (std::abs(nx) > epsilon) ? std::abs(dx / nx) : REAL_MAX;
            const real sdy = (std::abs(ny) > epsilon) ? std::abs(dy / ny) : REAL_MAX;
            const real sdz = (std::abs(nz) > epsilon) ? std::abs(dz / nz) : REAL_MAX;

            // Initialize distance to first boundary per axis
            const real xE = _grid.bx[i + (xdir + 1) / 2];
            const real yE = _grid.by[j + (ydir + 1) / 2];
            const real zE = _grid.bz[k + (zdir + 1) / 2];
            real       sx = (std::abs(nx) > epsilon) ? (xE - rx) / nx : REAL_MAX;
            real       sy = (std::abs(ny) > epsilon) ? (yE - ry) / ny : REAL_MAX;
            real       sz = (std::abs(nz) > epsilon) ? (zE - rz) / nz : REAL_MAX;

            // Traverse
            while (m >= 0 && m < N)
            {
                real ds = std::min({sx, sy, sz});

                const real accum_next = accum + ds;
                const bool interacts  = accum_next > target;
                if (interacts)
                {
                    ds    = target - accum;
                    accum = target;
                }
                else
                {
                    accum = accum_next;
                }

                rx += nx * ds;
                ry += ny * ds;
                rz += nz * ds;

                // attenuate
                const real dtau    = kappa[m] * ds;
                const real L_begin = weight / lambda;
                weight *= exp(-dtau);
                const real L_end = weight / lambda;

                // store radiation field
                const real Lds = (L_begin - L_end) / dtau;
                _rad(m, radWavIndex) += Lds;

                // don't propagate to next cell
                if (interacts)
                    break;

                if (sx <= sy && sx <= sz)
                {
                    i += xdir;
                    m += xdir * Nzy;
                    sx += sdx;
                }
                else if (sy <= sz)
                {
                    j += ydir;
                    m += ydir * Nz;
                    sy += sdy;
                }
                else
                {
                    k += zdir;
                    m += zdir;
                    sz += sdz;
                }
            }
        }
    );
}