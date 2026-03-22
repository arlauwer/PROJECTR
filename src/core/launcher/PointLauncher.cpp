#include "PointLauncher.hpp"
#include "Random.hpp"

void PointLauncher::launch(Batch& batch)
{
    batch.for_each(
        [&batch](size_t b)
        {
            batch.luminosity[b] = 1.0;
            batch.lambda[b]     = 1.0;

            batch.accum[b]  = 0.0;
            batch.target[b] = Random::exponential(1.0);

            batch.rx[b] = 0.;
            batch.ry[b] = 0.;
            batch.rz[b] = 0.;

            auto [nx, ny, nz] = Random::direction();
            batch.nx[b]       = nx;
            batch.ny[b]       = ny;
            batch.nz[b]       = nz;
        }
    );
}