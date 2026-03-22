#include "Simulation.hpp"
#include "grid/CartesianGrid.hpp"
#include "launcher/PointLauncher.hpp"
#include "scatterer/TestScatterer.hpp"

int main()
{
    Simulation sim(1, 1);

    const vector<real> borders = {1, 2};

    sim.setGrid(std::make_unique<CartesianGrid>(-1, 1, 100, borders));
    sim.setLauncher(std::make_unique<PointLauncher>());
    sim.setScatterer(std::make_unique<TestScatterer>());

    sim.setup();
    sim.run();

    return 0;
}