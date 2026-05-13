#pragma once

#include "Config.hpp"
#include <cmath>

namespace Math
{

inline void linspace(vector<real>& x, real begin, real end, size_t N)
{
    x.resize(N);
    real dx = (end - begin) / static_cast<real>(N - 1);
    for (size_t i = 0; i < N; i++)
        x[i] = begin + i * dx;
}

template <typename T, typename V>
inline void fill(vector<T>& x, V f)
{
    std::fill(x.begin(), x.end(), f);
}

template <typename T>
inline void clamp(T& x, T min, T max)
{
    x = std::min(std::max(x, min), max);
}

// normalized cumulative distribution function
inline real cdf(vector<real>& prob, vector<real>& cum)
{
    size_t N = prob.size();
    cum.resize(N + 1);
    cum[0] = 0.;
    for (size_t i = 0; i < N; i++)
        cum[i + 1] = cum[i] + prob[i];

    real sum = cum[N];
    for (size_t i = 0; i < N + 1; i++)
        cum[i] /= sum;

    return sum;
}

inline real cdfLogLog(vector<real>& x, vector<real>& p, vector<real>& C)
{
    size_t N = p.size();
    C.resize(N); // amount of segments + 1
    C[0] = 0.;

    real norm = 0.;
    for (size_t i = 0; i < N - 1; i++)
    {
        real p1 = p[i];
        real p2 = p[i + 1];
        real x1 = x[i];
        real x2 = x[i + 1];

        real slope = std::log10(p2 / p1) / std::log10(x2 / x1);
        real area;
        if (std::abs(slope + 1.) < 1e-10)
            area = p1 * std::log(x2 / x1);
        else
            area = p1 / (slope + 1.) * (x2 * std::pow(x2 / x1, slope) - x1);
        norm += area;
        C[i + 1] = norm;
    }

    for (real& c : C)
        c /= norm;

    return norm;
}

template <typename T>
inline int locateImpl(const vector<T>& vec, const T& val, int n)
{
    int jl = -1;
    int ju = n;
    while (ju - jl > 1)
    {
        int jm = (ju + jl) >> 1;
        if (val < vec[jm])
            ju = jm;
        else
            jl = jm;
    }
    return jl;
}

template <typename T>
inline int locate(const vector<T>& vec, T val)
{
    int n = vec.size();
    if (val == vec[n - 1])
        return n - 2;
    return locateImpl(vec, val, n);
}

template <typename T>
inline int locateClip(const vector<T>& vec, T val)
{
    int n = vec.size();
    if (val < vec[0])
        return 0;
    return locateImpl(vec, val, n - 1);
}

template <typename T>
inline int locateFail(const vector<T>& vec, T val)
{
    int n = vec.size();
    if (val > vec[n - 1])
        return -1;
    return locateImpl(vec, val, n - 1);
}

inline real gexp(real p, real x)
{
    const real q = 1.0 - p;
    if (q == 0.0)
        return std::exp(x);
    else if (std::fabs(q) < 1e-3)
    {
        real x2 = x * x;
        return std::exp(x) * (1.0 - 0.5 * x2 * q + 1.0 / 24.0 * x * x2 * (8.0 + 3.0 * x) * q * q -
                              1.0 / 48.0 * x2 * x2 * (12.0 + 8.0 * x + x2) * q * q * q);
    }
    else
        return std::pow(1.0 + q * x, 1.0 / q);
}

}; // namespace Math
