#pragma once

#include "../Batch.hpp"

class Scatterer
{
  public:
    virtual ~Scatterer() = default;

    virtual void scatter(Batch& batch) = 0;
};