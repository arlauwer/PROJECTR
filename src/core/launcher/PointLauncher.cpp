#include "PointLauncher.hpp"

PointLauncher::PointLauncher(real x, real y, real z)
    : _x(x),
      _y(y),
      _z(z)
{
}

void PointLauncher::launch(Batch& batch)
{
    batch.for_each(
        [this, &batch](size_t b)
        {
            batch.rx[b] = _x;
            batch.ry[b] = _y;
            batch.rz[b] = _z;
        }
    );
}

BatchField PointLauncher::provides() const
{
    return BatchField::Position;
}