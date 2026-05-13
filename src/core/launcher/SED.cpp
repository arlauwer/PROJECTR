#include "SED.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "TextInput.hpp"
#include <cmath>

namespace
{

constexpr real slopeTolerance = 1e-3;

#define POW(x) std::pow(x, slope + 1.)

} // namespace

SED::SED(const string& filepath)
{
    TextInput::read(filepath, _wav, _flux);

    _luminosity = Math::cdfLogLog(_wav, _flux, _cumFlux);
}

SED::SED(const vector<real>& wav, const vector<real>& flux)
    : _wav(wav),
      _flux(flux)
{
    _luminosity = Math::cdfLogLog(_wav, _flux, _cumFlux);
}

real SED::sample() const
{
    real X = Random::uniform();
    int  i = Math::locateClip(_cumFlux, X); // [0, N-2]

    real p1 = _flux[i];
    real p2 = _flux[i + 1];
    real x1 = _wav[i];
    real x2 = _wav[i + 1];

    real slope = std::log10(p2 / p1) / std::log10(x2 / x1);
    real wav;

    if (fabs(slope + 1) < slopeTolerance) // if slope is close to -1
    {
        wav = (X - _cumFlux[i]) * (std::log(x2) - std::log(x1)) / (_cumFlux[i + 1] - _cumFlux[i]) + std::log(x1);
        wav = std::exp(wav);
    }
    else
    {
        wav = (X - _cumFlux[i]) * (POW(x2) - POW(x1)) / (_cumFlux[i + 1] - _cumFlux[i]) + POW(x1);
        wav = std::pow(wav, 1. / (slope + 1.));
    }

    return wav;
}