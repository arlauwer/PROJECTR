#pragma once

#include "Launcher.hpp"

class PointLauncher : public Launcher
{
  public:
    void launch(Batch& batch) override;
};