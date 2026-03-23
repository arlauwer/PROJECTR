#pragma once

#include "Config.hpp"
#include "pcg/pcg_random.hpp"
#include <random>

namespace Random
{

// Use random_device to seed the rng
#ifdef DOUBLE_PRECISION
inline thread_local pcg64 rng(pcg_extras::seed_seq_from<std::random_device>{});
#else
inline thread_local pcg32 rng(pcg_extras::seed_seq_from<std::random_device>{});
#endif

// not sure if thread_local is needed here
inline thread_local std::uniform_real_distribution<real> dist(0.0, 1.0);

inline void seed(size_t seed)
{
    rng.seed(seed);
}

inline real uniform()
{
    return dist(rng);
}

inline real uniform(real m, real M)
{
    return m + (M - m) * uniform();
}

inline real exponential(real lambda)
{
    return -std::log(real(1.0) - uniform()) / lambda;
}

inline std::array<real, 3> direction()
{
    real u1  = uniform();
    real u2  = uniform();
    real z   = real(1.0) - real(2.0) * u1;
    real r   = std::sqrt(std::max(real(0.0), real(1.0) - z * z));
    real phi = real(2.0) * std::numbers::pi_v<real> * u2;
    return {r * std::cos(phi), r * std::sin(phi), z};
}

} // namespace Random