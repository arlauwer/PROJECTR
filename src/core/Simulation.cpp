
#include "Simulation.hpp"
#include "Log.hpp"
#include <cmath>

Simulation::Simulation() { }

void Simulation::setup()
{
    _batch.resize(16);

    _context.queryInfo();
}

void Simulation::run()
{
    // photon loop
    _launcher->launch(_batch);
    _propagator->propagate(_batch);
    // _scatterer->scatter(_batch);

    for (size_t i = 0; i < _batch.getSize(); i++)
    {
        real rx     = _batch.rx[i];
        real ry     = _batch.ry[i];
        real rz     = _batch.rz[i];
        real nx     = _batch.nx[i];
        real ny     = _batch.ny[i];
        real nz     = _batch.nz[i];
        real k      = std::sqrt(nx * nx + ny * ny + nz * nz);
        real weight = _batch.weight[i];
        real lambda = _batch.lambda[i];

        Log::debug(
            "Photon {}:\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}",
            i,
            rx,
            ry,
            rz,
            nx,
            ny,
            nz,
            k,
            weight,
            lambda
        );
    }
}