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

    auto& grid = sim.grid();
    Math::fill(grid.kappa(), 1.);
    Math::fill(grid.albedo(), 0.);

    sim.setup();
    sim.run();

    return 0;
}