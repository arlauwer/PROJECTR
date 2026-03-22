#pragma once

#include "../batch/Batch.hpp"

class Launcher
{
  public:
    virtual ~Launcher() = default;

    virtual void launch(Batch& batch) = 0;
};
