#include "IsotropicScatterer.hpp"
#include "Random.hpp"

void IsotropicScatterer::launch(Batch& batch)
{
    batch.for_each(
        [this, &batch](size_t b)
        {
            batch.accum[b]  = 0.0;
            batch.target[b] = Random::exponential(1.0);
        }
    );
}

void IsotropicScatterer::scatter(Batch& batch)
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

IsotropicScatterer::Capabilities IsotropicScatterer::provides() const
{
    return Capabilities::Target;
}