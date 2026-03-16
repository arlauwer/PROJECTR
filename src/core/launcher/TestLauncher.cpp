#include "TestLauncher.hpp"
#include "Random.hpp"

void TestLauncher::launch(Batch& batch)
{
    batch.for_each(
        [&batch](size_t b)
        {
            batch.weight[b] = 1.0;
            batch.lambda[b] = 1.0;

            batch.accum[b] = 0.0;
            // batch.target[b] = Random::exponential(1.0);
            batch.target[b] = 1e6;

            batch.rx[b] = Random::uniform(-1, 1);
            batch.ry[b] = Random::uniform(-1, 1);
            batch.rz[b] = Random::uniform(-1, 1);

            auto [nx, ny, nz] = Random::direction();
            batch.nx[b]       = nx;
            batch.ny[b]       = ny;
            batch.nz[b]       = nz;
        }
    );
}