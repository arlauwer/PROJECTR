#include "TestLauncher.hpp"
#include "Random.hpp"

void TestLauncher::launch(Batch& batch)
{
    batch.for_each(
        [&batch](size_t i)
        {
            batch.weight[i] = 1.0;
            batch.lambda[i] = 1.0;

            batch.acc[i] = 0.0;

            batch.rx[i] = Random::uniform(-1, 1);
            batch.ry[i] = Random::uniform(-1, 1);
            batch.rz[i] = Random::uniform(-1, 1);

            auto [kx, ky, kz] = Random::direction();
            batch.kx[i]       = kx;
            batch.ky[i]       = ky;
            batch.kz[i]       = kz;
        }
    );
}