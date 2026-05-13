#include "Random.hpp"
#include <gtest/gtest.h>

TEST(Random, DirectionIsIsotropic)
{
    Random::seed(0);
    const int N = 100000;

    double mean_x = 0, mean_y = 0, mean_z = 0;
    double mean_x2 = 0, mean_y2 = 0, mean_z2 = 0;

    for (int i = 0; i < N; i++)
    {
        auto [nx, ny, nz] = Random::direction();
        mean_x += nx;
        mean_y += ny;
        mean_z += nz;
        mean_x2 += nx * nx;
        mean_y2 += ny * ny;
        mean_z2 += nz * nz;
    }

    mean_x /= N;
    mean_y /= N;
    mean_z /= N;
    mean_x2 /= N;
    mean_y2 /= N;
    mean_z2 /= N;

    EXPECT_NEAR(mean_x, 0., 1e-2) << "x component biased";
    EXPECT_NEAR(mean_y, 0., 1e-2) << "y component biased";
    EXPECT_NEAR(mean_z, 0., 1e-2) << "z component biased";

    EXPECT_NEAR(mean_x2, 1. / 3., 1e-2) << "x^2 not 1/3";
    EXPECT_NEAR(mean_y2, 1. / 3., 1e-2) << "y^2 not 1/3";
    EXPECT_NEAR(mean_z2, 1. / 3., 1e-2) << "z^2 not 1/3";
}

TEST(Random, DirectionIsNormalized)
{
    Random::seed(0);
    for (int i = 0; i < 1000; i++)
    {
        auto [nx, ny, nz] = Random::direction();
        double len        = nx * nx + ny * ny + nz * nz;
        EXPECT_NEAR(len, 1.0, 1e-6) << "Direction not normalized at i=" << i << " len=" << len;
    }
}