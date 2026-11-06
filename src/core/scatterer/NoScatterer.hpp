#pragma once

#include "Scatterer.hpp"

#include <limits>

// Scatterer for pure absorption: photons never reach their target optical depth
class NoScatterer : public Scatterer
{
  public:
    void launch(Batch& batch) override
    {
        batch.for_each(
            [&batch](size_t b)
            {
                batch.accum[b]  = 0.;
                batch.target[b] = std::numeric_limits<real>::max();
            }
        );
    }

    void scatter(Batch& batch) override { }

    Capabilities provides() const override { return Capabilities::Target; }
};
