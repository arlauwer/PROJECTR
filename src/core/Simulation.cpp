
#include "Simulation.hpp"

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
}