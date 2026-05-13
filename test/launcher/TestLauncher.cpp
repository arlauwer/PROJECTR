#include "Config.hpp"
#include "launcher/SED.hpp"
#include <cmath>
#include <gtest/gtest.h>

namespace
{

constexpr size_t NUM_SAMPLES = 1000000;
constexpr real   tol         = 1e-2;

template <Real... Ts>
inline void compareMoments(real wav1, real wav2, real flux1, real flux2, Ts... moments)
{
    vector<real> wav  = {wav1, wav2};
    vector<real> flux = {flux1, flux2};
    SED          sed(wav, flux);

    // Collect samples
    vector<real> samples(NUM_SAMPLES);
    for (int i = 0; i < NUM_SAMPLES; i++)
        samples[i] = sed.sample();

    // calculate moments and compare to expected
    int  order         = 0; // start with first moment = normalization (luminosity)
    auto compareMoment = [&](real expected)
    {
        real moment = 0;
        if (order == 0)
        {
            moment = sed.luminosity();
        }
        else
        {
            for (real s : samples)
                moment += std::pow(s, order);
            moment /= NUM_SAMPLES;
        }

        EXPECT_NEAR(moment, expected, tol);
        order++;
    };

    // unfold variadic moments
    (compareMoment(moments), ...);
}

} // namespace

TEST(SED, CompareMoments)
{
    // analytical moments of power law with slope 1 (linear)
    compareMoments(1, 2, 1, 2, 1.500, 1.556, 2.500, 4.133, 7.000);

    // analytical moments of power law with slope 2 (quadratic)
    compareMoments(1, 2, 1, 4, 2.333, 1.607, 2.657, 4.500, 7.776);

    // analytical moments of power law with slope -1 (hyperbolic)
    compareMoments(1, 2, 2, 1, 1.386, 1.443, 2.164, 3.366);

    // analytical moments of power law with slope ~ -1 (~hyperbolic)
    compareMoments(1, 2, 2, 1.001, 1.387, 1.443, 2.164, 3.367);
}