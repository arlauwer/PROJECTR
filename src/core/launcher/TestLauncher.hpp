#pragma once

#include "Launcher.hpp"

class TestLauncher : public Launcher
{
  public:
    void launch(Batch& batch) override;
};