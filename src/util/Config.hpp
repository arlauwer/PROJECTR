#pragma once

#include <concepts>
#include <float.h>
#include <memory>
#include <string>
#include <vector>

#define DOUBLE_PRECISION

#ifdef DOUBLE_PRECISION
using real = double;
#define REAL_MAX DBL_MAX;
#else
using real = float;
#define REAL_MAX FLT_MAX;
#endif

using std::string;
using std::unique_ptr;
using std::vector;

template <typename T>
concept Real = std::convertible_to<T, real>;

template <typename T>
concept RealVector = std::convertible_to<T, vector<real>>;

template <typename T>
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}