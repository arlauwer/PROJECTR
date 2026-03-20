#pragma once

#include "Config.hpp"

#include <algorithm>
#include <cstddef>
#include <execution>
#include <sys/types.h>

struct Batch
{
    vector<real> weight;
    vector<real> lambda;

    vector<real> accum;
    vector<real> target;

    vector<real> rx;
    vector<real> ry;
    vector<real> rz;

    vector<real> nx;
    vector<real> ny;
    vector<real> nz;

    vector<int> m;
    vector<int> i;
    vector<int> j;
    vector<int> k;

    template <typename F> void for_each(F&& f)
    {
        std::vector<size_t> indices(size);
        std::iota(indices.begin(), indices.end(), 0);
#ifndef NDEBUG
        std::for_each(std::execution::seq, indices.begin(), indices.end(), std::forward<F>(f));
#else
        std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), std::forward<F>(f));
#endif
    }

    void resize(size_t size)
    {
        this->size = size;
        weight.resize(size, 0.);
        lambda.resize(size, 0.);

        accum.resize(size, 0.);
        target.resize(size, 0.);

        rx.resize(size, 0.);
        ry.resize(size, 0.);
        rz.resize(size, 0.);

        nx.resize(size, 0.);
        ny.resize(size, 0.);
        nz.resize(size, 0.);

        m.resize(size, 0);
        i.resize(size, 0);
        j.resize(size, 0);
        k.resize(size, 0);
    }

    size_t getSize() const
    {
        return size;
    }

  private:
    size_t size;
};