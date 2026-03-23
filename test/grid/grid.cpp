#include "Config.hpp"
#include "Random.hpp"
#include "grid/CartesianGrid.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <memory>

TEST(CartesianGrid, AccumulateRadiationField)
{
    Random::seed(0);

    vector<real>  borders = {1, 2}; // radiation field borders
    CartesianGrid cart(-1, 1, 3, borders);

    Math::fill(cart.kappa(), 1.);

    auto batch = cart.createBatch(1);

    batch->luminosity[0] = 1;
    batch->lambda[0]     = 1;
    batch->accum[0]      = 0.0;
    batch->target[0]     = REAL_MAX;
    batch->rx[0]         = -1 / 3.; // start on border
    batch->ry[0]         = 0;
    batch->rz[0]         = 0;
    batch->nx[0]         = 1;
    batch->ny[0]         = 0;
    batch->nz[0]         = 0;

    cart.initialize(*batch);
    cart.propagate(*batch);

    auto& rad = cart.radiationField();

    real dx    = 2. / 3.;
    real kappa = 1;
    real dtau  = kappa * dx;

    vector<real> RAD(3);

    real L_begin = 1;
    real L_end   = L_begin * exp(-dtau);

    RAD[0]  = 0;
    RAD[1]  = (L_begin - L_end) / dtau;
    L_begin = L_end;
    L_end   = L_begin * exp(-dtau);
    RAD[2]  = (L_begin - L_end) / dtau;

    for (int i = 0; i < 3; i++)
    {
        int m = cart.flattenIndex(i, 1, 1);
        EXPECT_NEAR(RAD[i], rad(m, 0), 1e-6) << "Failed at i=" << i << " m=" << m;
    }
}

TEST(CartesianGrid, ScatterDirection)
{
    constexpr real KAPPA = 4.;

    Random::seed(0);

    vector<real>  borders = {1, 2};
    CartesianGrid cart(-1, 1, 3, borders);
    Math::fill(cart.kappa(), KAPPA);
    Math::fill(cart.albedo(), 1.);

    auto batch = cart.createBatch(6);

    // all start at origin
    for (size_t b = 0; b < 6; b++)
    {
        batch->luminosity[b] = 1.;
        batch->lambda[b]     = 1.;
        batch->accum[b]      = 0.;
        batch->target[b]     = 0.5 * KAPPA; // scatter after ds=0.5
        batch->rx[b]         = 0.;
        batch->ry[b]         = 0.;
        batch->rz[b]         = 0.;
        batch->nx[b]         = 0.;
        batch->ny[b]         = 0.;
        batch->nz[b]         = 0.;
    }

    // 6 cardinal directions
    batch->nx[0] = 1;  // +x
    batch->nx[1] = -1; // -x
    batch->ny[2] = 1;  // +y
    batch->ny[3] = -1; // -y
    batch->nz[4] = 1;  // +z
    batch->nz[5] = -1; // -z

    cart.initialize(*batch);
    cart.propagate(*batch);

    // all should be at 0.5 along their axis
    EXPECT_NEAR(batch->rx[0], 0.5, 1e-6);
    EXPECT_NEAR(batch->rx[1], -0.5, 1e-6);
    EXPECT_NEAR(batch->ry[2], 0.5, 1e-6);
    EXPECT_NEAR(batch->ry[3], -0.5, 1e-6);
    EXPECT_NEAR(batch->rz[4], 0.5, 1e-6);
    EXPECT_NEAR(batch->rz[5], -0.5, 1e-6);

    // reset
    for (size_t b = 0; b < 6; b++)
    {
        batch->target[b] = REAL_MAX;
        batch->nx[b]     = 0;
        batch->ny[b]     = 0;
        batch->nz[b]     = 0;
    }

    // scatter orthogonally
    batch->ny[0] = 1;  // +y
    batch->ny[1] = -1; // -y
    batch->nz[2] = 1;  // +z
    batch->nz[3] = -1; // -z
    batch->nx[4] = 1;  // +x
    batch->nx[5] = -1; // -x

    cart.propagate(*batch);

    // previous axis unaffected
    EXPECT_NEAR(batch->rx[0], 0.5, 1e-6);
    EXPECT_NEAR(batch->rx[1], -0.5, 1e-6);
    EXPECT_NEAR(batch->ry[2], 0.5, 1e-6);
    EXPECT_NEAR(batch->ry[3], -0.5, 1e-6);
    EXPECT_NEAR(batch->rz[4], 0.5, 1e-6);
    EXPECT_NEAR(batch->rz[5], -0.5, 1e-6);

    // orthogonal axis affected
    EXPECT_NEAR(batch->ry[0], 1., 1e-6);
    EXPECT_NEAR(batch->ry[1], -1., 1e-6);
    EXPECT_NEAR(batch->rz[2], 1., 1e-6);
    EXPECT_NEAR(batch->rz[3], -1., 1e-6);
    EXPECT_NEAR(batch->rx[4], 1., 1e-6);
    EXPECT_NEAR(batch->rx[5], -1., 1e-6);

    for (size_t b = 0; b < 6; b++)
        EXPECT_EQ(batch->m[b], -1) << "Photon " << b << " should have exited";
}