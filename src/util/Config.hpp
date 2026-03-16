#pragma once

// #define EXECUTION_POLICY std::execution::par_unseq
#include <cstdint>
#include <sys/types.h>
#define EXECUTION_POLICY std::execution::seq

#include <float.h>
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
using std::vector;

inline uint32_t sign(real x)
{
    return (x > 0.) - (x < 0.);
}