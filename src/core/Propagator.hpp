#pragma once

#include "Batch.hpp"

class Propagator
{
  public:
    virtual void propagate(Batch& batch);
};