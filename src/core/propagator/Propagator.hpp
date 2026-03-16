#pragma once

#include "../Batch.hpp"

class Propagator
{
  public:
    virtual ~Propagator() = default;

    virtual void propagate(Batch& batch) = 0;
};