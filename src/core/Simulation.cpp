#include "Simulation.hpp"
#include "Log.hpp"
#include "grid/CartesianGrid.hpp"
#include "launcher/Launcher.hpp"
#include "scatterer/Scatterer.hpp"

Simulation::Simulation(size_t batch_size, size_t batch_count)
    : _batch_size(batch_size),
      _batch_count(batch_count)
{
}

void Simulation::setup()
{
    if (!_grid.get())
        throw std::runtime_error("Grid not set");

    if (!_launcher.get())
        throw std::runtime_error("Launcher not set");

    if (!_scatterer.get())
        throw std::runtime_error("Scatterer not set");

    _batch = _grid->createBatch(_batch_size);
    Log::info("Simulation setup finished");
}

void Simulation::run()
{
    for (size_t B = 0; B < _batch_count; B++)
    {
        // photon loop
        _launcher->launch(*_batch);
        _grid->initialize(*_batch);

        while (_batch->allAlive())
        {
            _grid->propagate(*_batch);
            _scatterer->scatter(*_batch);
        }
        Log::info("Photon batch {}/{} finished", B + 1, _batch_count);
    }

    CartesianGrid* cart = dynamic_cast<CartesianGrid*>(_grid.get());
    if (!cart)
        Log::error("Simulation::run(): Grid is not a CartesianGrid");

    cart->writeRadiationField("test.fits");
}

void Simulation::setGrid(unique_ptr<Grid> grid)
{
    if (!_grid.get())
        _grid = std::move(grid);
    else
        throw std::runtime_error("Grid already set");
}

void Simulation::setLauncher(unique_ptr<Launcher> launcher)
{
    if (!_launcher.get())
        _launcher = std::move(launcher);
    else
        throw std::runtime_error("Launcher already set");
}

void Simulation::setScatterer(unique_ptr<Scatterer> scatterer)
{
    if (!_scatterer.get())
        _scatterer = std::move(scatterer);
    else
        throw std::runtime_error("Scatterer already set");
}