#pragma once

#include "Launcher.hpp"

class LaserLauncher : public Launcher
{
  public:
    LaserLauncher(const string& filepath);

    void launch(Batch& batch) override;
};