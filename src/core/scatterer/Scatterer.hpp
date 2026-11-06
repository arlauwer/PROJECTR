#pragma once

#include "core/batch/Batch.hpp"

class Scatterer
{
  public:
    virtual ~Scatterer() = default;

    virtual void scatter(Batch& batch) = 0;
};