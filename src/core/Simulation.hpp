#pragma once

#include "VulkanContext.hpp"
#include "launcher/Launcher.hpp"
#include "propagator/Propagator.hpp"
#include "scatterer/Scatterer.hpp"
#include <memory>

class Simulation
{
  public:
    Simulation();

    virtual ~Simulation() = default;

    void setup();

    void run();

    void setLauncher(std::unique_ptr<Launcher> launcher)
    {
        if (!_launcher.get())
            _launcher = std::move(launcher);
    }

    void setPropagator(std::unique_ptr<Propagator> propagator)
    {
        if (!_propagator.get())
            _propagator = std::move(propagator);
    }

    void setScatterer(std::unique_ptr<Scatterer> scatterer)
    {
        if (!_scatterer.get())
            _scatterer = std::move(scatterer);
    }

  private:
    std::unique_ptr<Launcher>   _launcher;
    std::unique_ptr<Propagator> _propagator;
    std::unique_ptr<Scatterer>  _scatterer;

    Batch _batch;

    VulkanContext _context;
};
