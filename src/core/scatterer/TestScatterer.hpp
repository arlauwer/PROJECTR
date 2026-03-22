#pragma once

#include "Random.hpp"
#include "Scatterer.hpp"

template <Grid G>
class TestScatterer : public Scatterer<G>
{
  public:
    void scatter(G::Batch& batch) override
    {
        batch.for_each(
            [&batch](size_t b)
            {
                auto [nx, ny, nz] = Random::direction();
                batch.nx[b]       = nx;
                batch.ny[b]       = ny;
                batch.nz[b]       = nz;
            }
        );
    }
};