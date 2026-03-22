#pragma once

#include "../batch/Batch.hpp"
#include "../grid/Grid.hpp"
#include "Config.hpp"
#include "RadiationField.hpp"

class CartesianGrid : public Grid
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
    );

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
    unique_ptr<::Batch> createBatch(size_t size) override;
    void                initialize(::Batch& batch) override;
    void                propagate(::Batch& batch) override;

  private:
    // cartesian grid properties
    real         xmin, xmax, ymin, ymax, zmin, zmax;
    real         dx, dy, dz;
    size_t       Nx, Ny, Nz, Nzy, N;
    vector<real> bx;
    vector<real> by;
    vector<real> bz;

    // general grid properties
    vector<real>                  kappa;
    vector<real>                  albedo;
    std::optional<RadiationField> radField;
};