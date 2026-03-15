#pragma once

#include "Launcher.hpp"
#include "Propagator.hpp"
#include "Scatterer.hpp"
#include <memory>

class Simulation
{
  public:
    Simulation();

    void setup();

    void launch();

    void propagate();

    void scatter();

  private:
    std::unique_ptr<Launcher>   _launcher;
    std::unique_ptr<Propagator> _propagator;
    std::unique_ptr<Scatterer>  _scatterer;

    Batch _batch;
};
