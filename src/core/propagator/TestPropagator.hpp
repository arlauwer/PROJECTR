#pragma once

#include "Propagator.hpp"

class TestPropagator : public Propagator
{
  public:
    void propagate(Batch& batch) override;
};