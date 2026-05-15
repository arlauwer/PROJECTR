#pragma once

#include "../batch/Batch.hpp"
#include "../batch/BatchField.hpp"

class Launcher
{
  public:
    virtual ~Launcher() = default;

    virtual void launch(Batch& batch) = 0;

    virtual BatchField provides() const = 0;
};
