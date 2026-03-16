#pragma once

#include "CartesianGrid.hpp"
#include "Propagator.hpp"

class TestPropagator : public Propagator
{
  public:
    TestPropagator(real extent, size_t N);

    void propagate(Batch& batch) override;

  private:
    CartesianGrid _grid;
};