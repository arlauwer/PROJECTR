#pragma once

#include "Config.hpp"
#include <cstddef>

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

  private:
    size_t size;
};