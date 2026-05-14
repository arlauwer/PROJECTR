#include "../core/Simulation.hpp"
#include "../core/grid/CartesianGrid.hpp"
#include "../core/scatterer/NoScatterer.hpp"
#include "Random.hpp"
#include "TextInput.hpp"
#include "launcher/PointLauncher.hpp"
#include <gtest/gtest.h>

TEST(SKIRT, CompareToSKIRT)
{
    Random::seed(1);

    Simulation sim(1e6, 1);

    const vector<real> radBorders = {1e-6, 2e-6}; // 1 micron - 2 micron

    sim.setGrid(std::make_unique<CartesianGrid>(-1, 1, 1, radBorders));
    sim.setLauncher(std::make_unique<PointLauncher>(TEST_DIR "skirt/data/in/sed.txt"));
    sim.setScatterer(std::make_unique<NoScatterer>());

    auto& grid = sim.grid();
    Math::fill(grid.kappa(), 1e-6);
    Math::fill(grid.albedo(), 0.);

    sim.setup();
    sim.run();
    sim.finalize();

    // PROJECTR
    const auto& radField = grid.radiationField();

    // SKIRT
    vector<int>  SKIRT_cellIndices;
    vector<real> SKIRT_radField;
    TextInput::read(TEST_DIR "skirt/data/out/rad_rad_J.dat", SKIRT_cellIndices, SKIRT_radField);

    real rad  = radField(0, 0);
    real srad = SKIRT_radField[0];
    EXPECT_NEAR(rad, srad, 1);
}