#include "LaserLauncher.hpp"
#include "Random.hpp"

LaserLauncher::LaserLauncher(const string& filepath)
    : Launcher(SED(filepath))
{
}

void LaserLauncher::launch(Batch& batch)
{
    batch.for_each(
        [this, &batch](size_t b)
        {
            batch.luminosity[b] = 1. / (real)batch.size;
            batch.lambda[b]     = _sed.sample();

            batch.accum[b]  = 0.0;
            batch.target[b] = Random::exponential(1.0);

            batch.rx[b] = 0.;
            batch.ry[b] = 0.;
            batch.rz[b] = 0.;

            batch.nx[b] = 0.;
            batch.ny[b] = 0.;
            batch.nz[b] = 1.;
        }
    );
}