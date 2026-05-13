#pragma once

#include "Config.hpp"

struct SED
{
  public:
    SED(const string& filepath);

    SED(const vector<real>& wav, const vector<real>& flux);

    real sample() const;

    real luminosity() const
    {
        return _luminosity;
    }

  private:
    vector<real> _wav;        // wavelengths (N) [m]
    vector<real> _flux;       // flux at specified wavelengths (N) [W/m]
    vector<real> _cumFlux;    // cumulative flux assuming log-log interpolation (N-1)
    real         _luminosity; // integrated flux assuming log-log interpolation
};