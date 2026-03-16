#include "TestLauncher.hpp"
#include <cstddef>

void TestLauncher::launch(Batch& batch)
{
    batch.for_each(
        [&batch](size_t i)
        {
            batch.weight[i] = 1.0;
            batch.lambda[i] = 1.0;

            batch.acc[i] = 0.0;

            batch.rx[i] = 0.0;
            batch.ry[i] = 0.0;
            batch.rz[i] = 0.0;

            batch.kx[i] = 1.0;
            batch.ky[i] = 0.0;
            batch.kz[i] = 0.0;
        }
    );
}