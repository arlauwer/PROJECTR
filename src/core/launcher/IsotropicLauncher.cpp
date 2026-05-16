#include "IsotropicLauncher.hpp"
#include "Random.hpp"

void IsotropicLauncher::launch(Batch& batch)
{
    batch.for_each(
        [this, &batch](size_t b)
        {
            auto [nx, ny, nz] = Random::direction();
            batch.nx[b]       = nx;
            batch.ny[b]       = ny;
            batch.nz[b]       = nz;
        }
    );
}

Launcher::Capabilities IsotropicLauncher::provides() const
{
    return Capabilities::Direction;
}