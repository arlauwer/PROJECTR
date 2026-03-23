#include "../core/Simulation.hpp"
#include "../core/grid/CartesianGrid.hpp"
#include "../core/launcher/PointLauncher.hpp"
#include "../core/scatterer/TestScatterer.hpp"

int main()
{
    Simulation sim(65536, 10);

    const vector<real> borders = {1, 2};

    sim.setGrid(std::make_unique<CartesianGrid>(-1, 1, 100, borders));
    sim.setLauncher(std::make_unique<PointLauncher>());
    sim.setScatterer(std::make_unique<TestScatterer>());

    sim.setup();
    sim.run();

    return 0;
}