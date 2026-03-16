#include "TestPropagator.hpp"

void TestPropagator::propagate(Batch& batch)
{
    batch.for_each(
        [&batch](size_t i)
        {
            batch.rx[i] += batch.kx[i];
            batch.ry[i] += batch.ky[i];
            batch.rz[i] += batch.kz[i];
        }
    );
}