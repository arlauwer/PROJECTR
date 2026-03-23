#include "Config.hpp"
#include "Random.hpp"
#include "grid/CartesianGrid.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <memory>

TEST(CartesianGrid, propagate)
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