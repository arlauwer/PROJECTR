#pragma once

#include "Config.hpp"
#include <algorithm>
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

    vector<uint64_t> m;
    vector<uint32_t> i;
    vector<uint32_t> j;
    vector<uint32_t> k;

    template <typename F> void for_each(F&& f)
    {
        std::vector<size_t> indices(size);
        std::iota(indices.begin(), indices.end(), 0);
        std::for_each(EXECUTION_POLICY, indices.begin(), indices.end(), std::forward<F>(f));
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