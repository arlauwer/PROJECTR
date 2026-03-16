
#include "Simulation.hpp"
#include "log.hpp"

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
    _scatterer->scatter(_batch);

    for (size_t i = 0; i < _batch.getSize(); i++)
    {
        Log::debug("Photon {}: {} {} {}", i, _batch.rx[i], _batch.kx[i], _batch.weight[i]);
    }
}