#include "Simulation.hpp"
#include "grid/CartesianGrid.hpp"
#include "launcher/PointLauncher.hpp"
#include "scatterer/TestScatterer.hpp"

int main()
{
    Simulation<CartesianGrid> sim(16);

    const vector<real> borders = {1, 2};

    sim.setGrid(std::make_unique<CartesianGrid>(-1, 1, -1, 1, -1, 1, 3, 3, 3, borders));
    sim.setLauncher(std::make_unique<PointLauncher<CartesianGrid>>());
    sim.setScatterer(std::make_unique<TestScatterer<CartesianGrid>>());

    sim.setup();
    sim.run();

    return 0;
}