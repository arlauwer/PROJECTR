#include "TestScatterer.hpp"

void TestScatterer::scatter(Batch& batch)
{
    batch.for_each(
        [&batch](size_t b)
        {
            batch.nx[b] = -1;
            batch.ny[b] = 0;
            batch.nz[b] = 0;
        }
    );
}