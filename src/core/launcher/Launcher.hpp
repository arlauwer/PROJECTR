#pragma once

#include "../grid/Grid.hpp"

template <Grid G>
class Launcher
{
  public:
    virtual ~Launcher() = default;

    virtual void launch(G::Batch& batch) = 0;
};
