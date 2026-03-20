#pragma once

#include "CartesianGrid.hpp"
#include "Propagator.hpp"
#include "RadiationField.hpp"
#include "WavGrid.hpp"

class TestPropagator : public Propagator
{
  public:
    TestPropagator(real extent, size_t N);

    void propagate(Batch& batch) override;

    void test(real wavelength);

  private:
    CartesianGrid _grid;

    WavGrid        _radWavGrid;
    RadiationField _rad;
};