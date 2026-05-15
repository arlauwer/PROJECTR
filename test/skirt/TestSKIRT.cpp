#include "../core/Simulation.hpp"
#include "../core/grid/CartesianGrid.hpp"
#include "../core/scatterer/NoScatterer.hpp"
#include "../test/core/test.hpp"
#include "Random.hpp"
#include "TextInput.hpp"
#include "launcher/PointLauncher.hpp"
#include "scatterer/TestScatterer.hpp"
#include <gtest/gtest.h>

#define SKIRT_DIR TEST_DIR "skirt/data/"

TEST(SKIRT, SKIRTSingle)
{
    Random::seed(0);

    Simulation sim(1e6, 1);

    const vector<real> radBorders = {1e-6, 2e-6};

    sim.setGrid(std::make_unique<CartesianGrid>(-1, 1, 1, radBorders));
    sim.setLauncher(std::make_unique<PointLauncher>(SKIRT_DIR "single/in/sed.txt"));
    sim.setScatterer(std::make_unique<NoScatterer>());

    auto& grid = sim.grid();
    Math::fill(grid.kappa(), 10.);
    Math::fill(grid.albedo(), 0.);

    sim.setup();
    sim.run();
    sim.finalize();

    // PROJECTR
    const auto& radField = grid.radiationField();

    // SKIRT
    vector<int>  SKIRT_cellIndices;
    vector<real> SKIRT_radField;
    TextInput::read(SKIRT_DIR "single/out/rad_rad_J.dat", SKIRT_cellIndices, SKIRT_radField);

    real rad  = radField(0, 0);
    real srad = SKIRT_radField[0];
    EXPECT_NEAR_REL(rad, srad, 1e-2);
}

TEST(SKIRT, SKIRTScatter)
{
    Random::seed(0);

    Simulation sim(1e6, 1);

    const vector<real> radBorders = {1e-6, 2e-6};

    sim.setGrid(std::make_unique<CartesianGrid>(-1, 1, 1, radBorders));
    sim.setLauncher(std::make_unique<PointLauncher>(SKIRT_DIR "scatter/in/sed.txt"));
    sim.setScatterer(std::make_unique<TestScatterer>());

    auto& grid = sim.grid();
    Math::fill(grid.kappa(), 2.);
    Math::fill(grid.albedo(), 0.5);

    sim.setup();
    sim.run();
    sim.finalize();

    // PROJECTR
    const auto& radField = grid.radiationField();

    // SKIRT
    vector<int>  SKIRT_cellIndices;
    vector<real> SKIRT_radField;
    TextInput::read(SKIRT_DIR "scatter/out/rad_rad_J.dat", SKIRT_cellIndices, SKIRT_radField);

    real rad  = radField(0, 0);
    real srad = SKIRT_radField[0];
    EXPECT_NEAR_REL(rad, srad, 1e-2);
}

TEST(SKIRT, SKIRTMulti)
{
    Random::seed(0);

    Simulation sim(1e6, 1);

    const vector<real> radBorders = {1e-6, 2e-6, 3e-6};

    sim.setGrid(std::make_unique<CartesianGrid>(-1, 1, 3, radBorders));
    sim.setLauncher(std::make_unique<PointLauncher>(SKIRT_DIR "multi/in/sed.txt"));
    sim.setScatterer(std::make_unique<TestScatterer>());

    auto& grid = sim.grid();
    Math::fill(grid.kappa(), 2.);
    Math::fill(grid.albedo(), 2. / 3.);

    sim.setup();
    sim.run();
    sim.finalize();

    // PROJECTR
    const auto& radField = grid.radiationField();

    // SKIRT
    vector<int>  SKIRT_cellIndices;
    vector<real> SKIRT_radField1;
    vector<real> SKIRT_radField2;
    TextInput::read(SKIRT_DIR "multi/out/rad_rad_J.dat", SKIRT_cellIndices, SKIRT_radField1, SKIRT_radField2);

    EXPECT_EQ(grid.numCells(), SKIRT_radField1.size());
    EXPECT_EQ(grid.numCells(), SKIRT_radField2.size());

    for (size_t m = 0; m < grid.numCells(); m++)
    {
        real rad1  = radField(m, 0);
        real rad2  = radField(m, 1);
        real srad1 = SKIRT_radField1[m];
        real srad2 = SKIRT_radField2[m];
        EXPECT_NEAR_REL(rad1, srad1, 5e-2);
        EXPECT_NEAR_REL(rad2, srad2, 5e-2);
    }
}