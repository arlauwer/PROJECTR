#include "Simulation.hpp"
#include "launcher/TestLauncher.hpp"
#include "propagator/TestPropagator.hpp"
#include "scatterer/TestScatterer.hpp"

int main()
{
    Simulation sim;

    sim.setLauncher(std::make_unique<TestLauncher>());
    sim.setPropagator(std::make_unique<TestPropagator>());
    sim.setScatterer(std::make_unique<TestScatterer>());

    sim.setup();
    sim.run();

    return 0;
}