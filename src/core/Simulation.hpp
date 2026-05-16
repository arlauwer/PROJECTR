#pragma once

#include "Log.hpp"
#include "batch/Batch.hpp"
#include "grid/Grid.hpp"
#include <concepts>

class Launcher;
class Scatterer;

class Simulation
{
  public:
    Simulation(size_t batch_size, size_t batch_count);

    virtual ~Simulation() = default;

    void setup();

    void run();

    void finalize();

    template <std::derived_from<Grid> T, typename... Args>
    void setGrid(Args&&... args)
    {
        if (!_grid.get())
            _grid = std::make_unique<T>(std::forward<Args>(args)...);
        else
            Log::fatal("Grid already set");
    }

    template <std::derived_from<Launcher> T, typename... Args>
    void addLauncher(Args&&... args)
    {
        _launchers.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template <std::derived_from<Scatterer> T, typename... Args>
    void setScatterer(Args&&... args)
    {
        if (!_scatterer.get())
            _scatterer = std::make_unique<T>(std::forward<Args>(args)...);
        else
            Log::fatal("Scatterer already set");
    }

  private:
    void checkGrid();

    void checkLaunchers();

    void checkScatterer();

    unique_ptr<Grid> _grid;

    vector<unique_ptr<Launcher>> _launchers;

    unique_ptr<Scatterer> _scatterer;

    unique_ptr<Batch> _batch;

    size_t _batch_size, _batch_count;
};
