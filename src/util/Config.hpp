#pragma once

#include <float.h>
#include <memory>
#include <string>
#include <vector>

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
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}