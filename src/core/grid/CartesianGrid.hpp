#pragma once

#include "../batch/Batch.hpp"
#include "../grid/Grid.hpp"
#include "Config.hpp"
#include "RadiationField.hpp"
#include <cmath>
#include <optional>

class CartesianGrid
{

  public:
    CartesianGrid(
        real                xmin,
        real                xmax,
        real                ymin,
        real                ymax,
        real                zmin,
        real                zmax,
        size_t              Nx,
        size_t              Ny,
        size_t              Nz,
        const vector<real>& borders
    )
        : xmin(xmin),
          xmax(xmax),
          ymin(ymin),
          ymax(ymax),
          zmin(zmin),
          zmax(zmax),
          dx((xmax - xmin) / static_cast<real>(Nx)),
          dy((ymax - ymin) / static_cast<real>(Ny)),
          dz((zmax - zmin) / static_cast<real>(Nz)),
          Nx(Nx),
          Ny(Ny),
          Nz(Nz),
          Nzy(Nz * Ny)
    {
        bx.resize(Nx + 1);
        by.resize(Ny + 1);
        bz.resize(Nz + 1);

        Math::linspace(bx, xmin, xmax, Nx + 1);
        Math::linspace(by, ymin, ymax, Ny + 1);
        Math::linspace(bz, zmin, zmax, Nz + 1);

        N = Nx * Ny * Nz;

        // always add a radiation field
        addRadiationField(borders);
    }

    // cartesian batch
    struct Batch : public ::Batch
    {
        Batch(size_t size)
            : ::Batch(size),
              i(size, 0),
              j(size, 0),
              k(size, 0)
        {
        }

        vector<int> i;
        vector<int> j;
        vector<int> k;
    };

    // batch functions
    void initialize(Batch& batch)
    {
        batch.for_each(
            [this, &batch](size_t b)
            {
                real& rx = batch.rx[b];
                real& ry = batch.ry[b];
                real& rz = batch.rz[b];
                int&  i  = batch.i[b];
                int&  j  = batch.j[b];
                int&  k  = batch.k[b];
                int&  m  = batch.m[b];

                // Initialize cell indices
                i = static_cast<int>(std::floor((rx - xmin) / dx));
                j = static_cast<int>(std::floor((ry - ymin) / dy));
                k = static_cast<int>(std::floor((rz - zmin) / dz));
                m = i * Nzy + j * Nz + k;
            }
        );
    }

    void propagate(Batch& batch)
    {
        batch.for_each(
            [this, &batch](size_t b)
            {
                constexpr real epsilon = 1e-15;

                real&       weight = batch.luminosity[b];
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

                if (m < 0 || m >= N)
                    return;

                // radiation field wavelength grid index
                const size_t radWavIndex = radField->wavGrid.index(lambda);

                // Initialize step directions
                const int xdir = sign(nx);
                const int ydir = sign(ny);
                const int zdir = sign(nz);

                // Initialize distance to cross boundary per axis
                const real sdx = (std::abs(nx) > epsilon) ? std::abs(dx / nx) : REAL_MAX;
                const real sdy = (std::abs(ny) > epsilon) ? std::abs(dy / ny) : REAL_MAX;
                const real sdz = (std::abs(nz) > epsilon) ? std::abs(dz / nz) : REAL_MAX;

                // Initialize distance to first boundary per axis
                const real xE = bx[i + (xdir + 1) / 2];
                const real yE = by[j + (ydir + 1) / 2];
                const real zE = bz[k + (zdir + 1) / 2];
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
                    (*radField)(m, radWavIndex) += Lds;

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

    // radiation field support
    void addRadiationField(const vector<real>& borders)
    {
        if (radField)
            throw std::runtime_error("Radiation field already exists");

        radField.emplace(N, borders.size() - 1, borders);
    }

    // required grid members
  public:
    size_t                        N;
    vector<real>                  kappa;
    vector<real>                  albedo;
    std::optional<RadiationField> radField;

  private:
    // cartesian grid properties
    real         xmin, xmax, ymin, ymax, zmin, zmax;
    real         dx, dy, dz;
    size_t       Nx, Ny, Nz, Nzy;
    vector<real> bx;
    vector<real> by;
    vector<real> bz;
};

static_assert(Grid<CartesianGrid>, "CartesianGrid must satisfy GridType");