#pragma once

#include "Launcher.hpp"

class IsotropicLauncher : public Launcher
{
  public:
    void launch(Batch& batch) override;

    Capabilities provides() const override;
};