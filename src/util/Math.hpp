#pragma once

#include "Config.hpp"

namespace Math
{

template <typename T>
inline void linspace(vector<T>& x, T a, T b, size_t N)
{
    x.resize(N);
    T dx = (b - a) / static_cast<T>(N);
    for (size_t i = 0; i < N; i++)
        x[i] = a + i * dx;
}

template <typename T, typename U>
inline void fill(vector<T>& x, U f)
{
    std::fill(x.begin(), x.end(), f);
}

}; // namespace Math
