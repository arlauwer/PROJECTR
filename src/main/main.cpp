#include "../core/Simulation.hpp"
#include "../core/grid/CartesianGrid.hpp"
#include "../core/launcher/PointLauncher.hpp"
#include "../core/scatterer/TestScatterer.hpp"

int main()
{
    Simulation sim(1e6, 1);

    const real         L       = 1.;
    const vector<real> borders = {0.999, 1.001};

    sim.setGrid(std::make_unique<CartesianGrid>(-1, 1, 20, borders));
    sim.setLauncher(std::make_unique<PointLauncher>("skirt/in/sed.txt"));
    sim.setScatterer(std::make_unique<TestScatterer>());

    auto& grid = sim.grid();
    Math::fill(grid.kappa(), 1.);
    Math::fill(grid.albedo(), 0.);

    sim.setup();
    sim.run();
    sim.finalize();

    return 0;
}