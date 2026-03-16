
#include "Simulation.hpp"
#include "log.hpp"
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
    // _propagator->propagate(_batch);
    // _scatterer->scatter(_batch);

    for (size_t i = 0; i < _batch.getSize(); i++)
    {
        double rx     = _batch.rx[i];
        double ry     = _batch.ry[i];
        double rz     = _batch.rz[i];
        double kx     = _batch.kx[i];
        double ky     = _batch.ky[i];
        double kz     = _batch.kz[i];
        double k      = std::sqrt(kx * kx + ky * ky + kz * kz);
        double weight = _batch.weight[i];
        double lambda = _batch.lambda[i];

        Log::debug("Photon {}:\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}", i, rx, ry, rz, kx, ky, kz, k, weight, lambda);
    }
}