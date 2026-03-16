#include "TestScatterer.hpp"

void TestScatterer::scatter(Batch& batch)
{
    batch.for_each(
        [&batch](size_t i)
        {
            batch.kx[i] = -1;
            batch.ky[i] = 0;
            batch.kz[i] = 0;
        }
    );
}