#include "../core/Simulation.hpp"
#include "../core/grid/CartesianGrid.hpp"
#include "../core/launcher/IsotropicLauncher.hpp"
#include "../core/launcher/PointLauncher.hpp"
#include "../core/launcher/SEDLauncher.hpp"
#include "../core/scatterer/IsotropicScatterer.hpp"

int main()
{
    Simulation sim(1e6, 1);

    const real         L       = 1.;
    const vector<real> borders = {0.999, 1.001};

    sim.setGrid<CartesianGrid>(-1, 1, 20, borders);

    sim.addLauncher<PointLauncher>(0., 0., 0.); // position
    sim.addLauncher<IsotropicLauncher>();       // direction
    sim.addLauncher<SEDLauncher>("sed.txt");    // luminosity | wavelength

    sim.setScatterer<IsotropicScatterer>();

    // auto& grid = sim.grid();
    // Math::fill(grid.kappa(), 1.);
    // Math::fill(grid.albedo(), 0.);

    sim.setup();
    sim.run();
    sim.finalize();

    return 0;
}