#pragma once

#include "Config.hpp"
#include <algorithm>
#include <execution>

struct Batch
{
    vector<real> weight;
    vector<real> lambda;

    vector<real> acc;

    vector<real> rx;
    vector<real> ry;
    vector<real> rz;

    vector<real> kx;
    vector<real> ky;
    vector<real> kz;

    template <typename F> void for_each(F&& f)
    {
        std::vector<size_t> indices(size);
        std::iota(indices.begin(), indices.end(), 0);
        std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), std::forward<F>(f));
    }

    void resize(size_t size)
    {
        this->size = size;
        weight.resize(size, 0.);
        lambda.resize(size, 0.);

        acc.resize(size, 0.);

        rx.resize(size, 0.);
        ry.resize(size, 0.);
        rz.resize(size, 0.);

        kx.resize(size, 0.);
        ky.resize(size, 0.);
        kz.resize(size, 0.);
    }

    size_t getSize() const
    {
        return size;
    }

  private:
    size_t size;
};