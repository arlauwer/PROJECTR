#pragma once

#include "batch/Batch.hpp"
#include "grid/Grid.hpp"

class Launcher;
class Scatterer;

class Simulation
{
  public:
    Simulation(size_t batch_size, size_t batch_count);

    virtual ~Simulation() = default;

    void setup();

    void run();

    void setGrid(unique_ptr<Grid> grid);

    void setLauncher(unique_ptr<Launcher> launcher);

    void setScatterer(unique_ptr<Scatterer> scatterer);

    Grid& grid();

    Launcher& launcher();

    Scatterer& scatterer();

  private:
    unique_ptr<Grid>      _grid;
    unique_ptr<Launcher>  _launcher;
    unique_ptr<Scatterer> _scatterer;

    unique_ptr<Batch> _batch;

    size_t _batch_size, _batch_count;
};
