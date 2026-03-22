#include "TestScatterer.hpp"
#include "Random.hpp"

void TestScatterer::scatter(Batch& batch)
{
    batch.for_each(
        [&batch](size_t b)
        {
            auto [nx, ny, nz] = Random::direction();
            batch.nx[b]       = nx;
            batch.ny[b]       = ny;
            batch.nz[b]       = nz;

            batch.accum[b]  = 0.;
            batch.target[b] = Random::exponential(1.0);
        }
    );
}