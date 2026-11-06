#include "SEDLauncher.hpp"

SEDLauncher::SEDLauncher(const string& filepath)
    : _sed(filepath)
{
}

void SEDLauncher::launch(Batch& batch)
{
    batch.for_each(
        [this, &batch](size_t b)
        {
            batch.luminosity[b] = 1. / (real)batch.size;
            batch.lambda[b]     = _sed.sample();
        }
    );
}

Launcher::Capabilities SEDLauncher::provides() const
{
    return Capabilities::Luminosity | Capabilities::Wavelength;
}