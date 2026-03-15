#include "Simulation.hpp"

Simulation::Simulation() { }

void Simulation::setup()
{
    _batch.resize(1024);
}

void Simulation::launch()
{
    _launcher->launch(_batch);
}

void Simulation::propagate()
{
    _propagator->propagate(_batch);
}

void Simulation::scatter()
{
    _scatterer->scatter(_batch);
}