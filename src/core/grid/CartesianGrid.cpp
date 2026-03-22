#include "CartesianGrid.hpp"
#include "FitsImage.hpp"
#include <cmath>

CartesianGrid::CartesianGrid(real dmin, real dmax, size_t N, const vector<real>& borders)
    : CartesianGrid(dmin, dmax, dmin, dmax, dmin, dmax, N, N, N, borders)
{
}

CartesianGrid::CartesianGrid(
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
    : _xmin(xmin),
      _xmax(xmax),
      _ymin(ymin),
      _ymax(ymax),
      _zmin(zmin),
      _zmax(zmax),
      _dx((xmax - xmin) / static_cast<real>(Nx)),
      _dy((ymax - ymin) / static_cast<real>(Ny)),
      _dz((zmax - zmin) / static_cast<real>(Nz)),
      _Nx(Nx),
      _Ny(Ny),
      _Nz(Nz),
      _Nzy(Nz * Ny)
{
    _bx.resize(Nx + 1);
    _by.resize(Ny + 1);
    _bz.resize(Nz + 1);

    Math::linspace(_bx, xmin, xmax, Nx + 1);
    Math::linspace(_by, ymin, ymax, Ny + 1);
    Math::linspace(_bz, zmin, zmax, Nz + 1);

    _N = Nx * Ny * Nz;

    _kappa.resize(_N, 1.0);
    _albedo.resize(_N, 0.5);

    // force enable radiation field
    _radField.emplace(_N, borders.size() - 1, borders);
}

unique_ptr<Batch> CartesianGrid::createBatch(size_t size)
{
    return std::make_unique<Batch>(size);
}

// batch functions
void CartesianGrid::initialize(::Batch& base)
{
    Batch& batch = static_cast<Batch&>(base);

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
            i = static_cast<int>(std::floor((rx - _xmin) / _dx));
            j = static_cast<int>(std::floor((ry - _ymin) / _dy));
            k = static_cast<int>(std::floor((rz - _zmin) / _dz));
            m = flattenIndex(i, j, k);
        }
    );
}

void CartesianGrid::propagate(::Batch& base)
{
    Batch& batch = static_cast<Batch&>(base);

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

            if (m < 0)
                return;

            // radiation field wavelength grid index
            const size_t radWavIndex = _radField->wavGrid.index(lambda);

            // Initialize step directions
            const int xdir = sign(nx);
            const int ydir = sign(ny);
            const int zdir = sign(nz);

            // Traverse
            while (true)
            {
                // determine distance to next boundary
                const real xE = _bx[i + (xdir + 1) / 2];
                const real yE = _by[j + (ydir + 1) / 2];
                const real zE = _bz[k + (zdir + 1) / 2];
                const real sx = (std::abs(nx) > epsilon) ? (xE - rx) / nx : REAL_MAX;
                const real sy = (std::abs(ny) > epsilon) ? (yE - ry) / ny : REAL_MAX;
                const real sz = (std::abs(nz) > epsilon) ? (zE - rz) / nz : REAL_MAX;
                real       ds = std::min({sx, sy, sz});

                // determine if interacts
                const real dtau       = _kappa[m] * ds;
                const real accum_next = accum + dtau;
                const bool interacts  = accum_next > target;
                if (interacts)
                {
                    // distance to interaction
                    ds    = target - accum;
                    accum = target;
                }
                else
                {
                    accum = accum_next;
                }

                // propagate
                rx += nx * ds;
                ry += ny * ds;
                rz += nz * ds;

                // attenuate
                const real L_begin = weight / lambda;
                weight *= exp(-dtau);
                const real L_end = weight / lambda;

                // store radiation field
                const real Lds = dtau > epsilon ? (L_begin - L_end) / dtau : 0.;
                (*_radField)(m, radWavIndex) += Lds;

                // don't update cell indices
                if (interacts)
                    break;

                // update cell indices
                if (sx <= sy && sx <= sz)
                {
                    i += xdir;
                    m += xdir * _Nzy;
                }
                else if (sy <= sz)
                {
                    j += ydir;
                    m += ydir * _Nz;
                }
                else
                {
                    k += zdir;
                    m += zdir;
                }

                if (!inside(i, j, k))
                {
                    m = -1; // photon exited grid
                    break;
                }
            }
        }
    );
}

void CartesianGrid::writeRadiationField(const string& filepath) const
{
    FitsImage image(_Nx, _Ny, _Nz);

    for (size_t i = 0; i < _Nx; i++)
    {
        for (size_t j = 0; j < _Ny; j++)
        {
            for (size_t k = 0; k < _Nz; k++)
            {
                int m          = flattenIndex(i, j, k);
                image(i, j, k) = (*_radField)(m, 0);
            }
        }
    }

    image.save(filepath);
}