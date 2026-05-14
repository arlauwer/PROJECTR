#pragma once

#include "Scatterer.hpp"

class NoScatterer : public Scatterer
{
  public:
    void scatter(Batch& batch) override { }
};