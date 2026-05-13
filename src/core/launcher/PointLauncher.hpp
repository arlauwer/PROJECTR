#pragma once

#include "Launcher.hpp"

class PointLauncher : public Launcher
{
  public:
    PointLauncher(const string& filepath);

    void launch(Batch& batch) override;
};