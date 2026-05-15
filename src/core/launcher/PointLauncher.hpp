#pragma once

#include "Launcher.hpp"

class PointLauncher : public Launcher
{
  public:
    PointLauncher(real x, real y, real z);

    void launch(Batch& batch) override;

    BatchField provides() const override;

  private:
    real _x, _y, _z;
};