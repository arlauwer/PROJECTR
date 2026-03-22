#pragma once

#include "Config.hpp"

#include <algorithm>
#include <cstddef>
#include <execution>
#include <sys/types.h>

struct Batch
{
  protected:
    Batch(size_t size)
        : size(size),
          luminosity(size, 0.),
          lambda(size, 0.),
          accum(size, 0.),
          target(size, 0.),
          rx(size, 0.),
          ry(size, 0.),
          rz(size, 0.),
          nx(size, 0.),
          ny(size, 0.),
          nz(size, 0.),
          m(size, -1)
    {
    }

  public:
    const size_t size;

    vector<real> luminosity;
    vector<real> lambda;

    vector<real> accum;  // accumulated: optical depth, etc.
    vector<real> target; // target: optical depth, etc.

    vector<real> rx;
    vector<real> ry;
    vector<real> rz;

    vector<real> nx;
    vector<real> ny;
    vector<real> nz;

    vector<int> m;

    template <typename F>
    void for_each(F&& f)
    {
        vector<size_t> indices(size);
        std::iota(indices.begin(), indices.end(), 0);
        std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), std::forward<F>(f));
    }

    bool allAlive() const
    {
        return std::all_of(m.begin(), m.end(), [](int mb) { return mb >= 0; });
    }
};