#pragma once

#include "../batch/Batch.hpp"
#include "../grid/Grid.hpp"
#include "Config.hpp"

class CartesianGrid : public Grid
{
  public:
    CartesianGrid(real dmin, real dmax, size_t N, const vector<real>& borders);

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

    // grid helper functions
    int flattenIndex(int i, int j, int k) const
    {
        return i * _Nzy + j * _Nz + k;
    }

    void writeRadiationField(const string& filepath) const;

  private:
    // cartesian grid properties
    real         _xmin, _xmax, _ymin, _ymax, _zmin, _zmax;
    real         _dx, _dy, _dz;
    size_t       _Nx, _Ny, _Nz, _Nzy, _N;
    vector<real> _bx;
    vector<real> _by;
    vector<real> _bz;
};