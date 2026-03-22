#pragma once

#include "VulkanContext.hpp"
#include "launcher/Launcher.hpp"
#include "scatterer/Scatterer.hpp"
#include <memory>

template <Grid G>
class Simulation
{
  public:
    Simulation(size_t batch_size)
        : _batch(batch_size)
    {
    }

    virtual ~Simulation() = default;

    void setup()
    {
        _context.queryInfo();
    }

    void run()
    {
        // photon loop
        _launcher->launch(_batch);
        _scatterer->scatter(_batch);
    }

    void setGrid(std::unique_ptr<G> grid)
    {
        if (!_grid.get())
            _grid = std::move(grid);
    }

    void setLauncher(std::unique_ptr<Launcher<G>> launcher)
    {
        if (!_launcher.get())
            _launcher = std::move(launcher);
    }

    void setScatterer(std::unique_ptr<Scatterer<G>> scatterer)
    {
        if (!_scatterer.get())
            _scatterer = std::move(scatterer);
    }

  private:
    std::unique_ptr<G>            _grid;
    std::unique_ptr<Launcher<G>>  _launcher;
    std::unique_ptr<Scatterer<G>> _scatterer;

    G::Batch _batch;

    VulkanContext _context;
};
