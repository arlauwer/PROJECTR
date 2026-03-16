#pragma once

#include "Scatterer.hpp"

class TestScatterer : public Scatterer
{
  public:
    void scatter(Batch& batch) override;
};