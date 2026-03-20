#pragma once

#include <algorithm>

namespace Math
{

inline double lnmean(double x1, double x2, double lnx1, double lnx2)
{
    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(lnx1, lnx2);
    }
    if (x1 <= 0)
        return 0.;

    double x = x2 / x1 - 1.;
    if (x < 1e-3)
    {
        return x1 / (1. - 1. / 2. * x + 1. / 3. * x * x - 1. / 4. * x * x * x + 1. / 5. * x * x * x * x -
                     1. / 6. * x * x * x * x * x);
    }
    else
    {
        return (x2 - x1) / (lnx2 - lnx1);
    }
}

} // namespace Math