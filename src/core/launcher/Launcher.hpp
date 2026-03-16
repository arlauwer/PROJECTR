#pragma once

#include "../Batch.hpp"

class Launcher
{
  public:
    virtual ~Launcher() = default;

    virtual void launch(Batch& batch) = 0;
};
