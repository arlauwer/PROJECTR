#include "Simulation.hpp"
#include "Config.hpp"
#include "batch/Batch.hpp"
#include "batch/BatchField.hpp"
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
    // grid
    if (!_grid.get())
        Log::fatal("Grid not set");

    // launchers
    BatchField required =
        BatchField::Position | BatchField::Direction | BatchField::Luminosity | BatchField::Wavelength;

    for (auto& launcher : _launchers)
    {
        BatchField provided = launcher->provides();

        // if provided present in required
        if ((required & provided) != BatchField::None)
        {
            required &= ~provided; // remove provided from required
        }
        else
        {
            // either not required or duplcate
            Log::fatal("Launchers provided unrequired/duplicate field: {}", to_string(provided));
        }
    }
    if (required != BatchField::None)
    {
        Log::fatal("Launchers missing required field: {}", to_string(required));
    }

    // scatterer
    if (!_scatterer.get())
        Log::fatal("Scatterer not set");

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
