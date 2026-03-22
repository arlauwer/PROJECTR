#pragma once

#include "../grid/Grid.hpp"

template <Grid G>
class Scatterer
{
  public:
    virtual ~Scatterer() = default;

    virtual void scatter(G::Batch& batch) = 0;
};