#include "Simulation.hpp"
#include "Config.hpp"
#include "batch/Batch.hpp"
#include "launcher/Launcher.hpp"
#include "scatterer/Scatterer.hpp"
#include <memory>

Simulation::Simulation(size_t batch_size, size_t batch_count)
    : _batch_size(batch_size),
      _batch_count(batch_count)
{
}

void Simulation::setup()
{
    checkGrid();
    checkLaunchers();
    checkScatterer();

    _batch = _grid->createBatch(_batch_size);
    Log::info("Simulation setup finished");
}

void Simulation::run()
{
    for (size_t B = 0; B < _batch_count; B++)
    {
        // photon loop
        for (auto& launcher : _launchers)
            launcher->launch(*_batch);

        _grid->launch(*_batch);

        while (_batch->anyAlive())
        {
            _grid->propagate(*_batch);
            _scatterer->scatter(*_batch);
        }
        Log::info("Photon batch {}/{} finished", B + 1, _batch_count);
    }
}

void Simulation::finalize()
{
    _grid->finalize();
}

void Simulation::checkGrid()
{
    if (!_grid.get())
        Log::fatal("Grid not set");
}

void Simulation::checkLaunchers()
{
    auto required = Launcher::Capabilities::All;

    for (auto& launcher : _launchers)
    {
        auto provided = launcher->provides();

        // if provided (exactly) present in required
        if ((required & provided) == provided)
        {
            required &= ~provided; // remove provided from required
        }
        else
        {
            // either not required or duplcate
            Log::fatal("Launcher provided unrequired/duplicate field: {}", Launcher::to_string(provided));
        }
    }
    if (required != Launcher::Capabilities::None)
    {
        Log::fatal("Launchers missing required field: {}", Launcher::to_string(required));
    }
}

void Simulation::checkScatterer()
{
    if (!_scatterer.get())
        Log::fatal("Scatterer not set");

    auto required = Scatterer::Capabilities::All;
    auto provided = _scatterer->provides();
    if (required != provided)
    {
        Log::fatal("Scatterer provided unrequired/duplicate field: {}", Scatterer::to_string(provided));
    }
}