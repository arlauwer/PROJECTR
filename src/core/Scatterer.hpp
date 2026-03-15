#pragma once

#include "Batch.hpp"

class Scatterer
{
  public:
    virtual void scatter(Batch& batch);
};