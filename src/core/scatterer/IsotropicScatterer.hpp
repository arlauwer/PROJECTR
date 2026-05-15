#pragma once

#include "Scatterer.hpp"

class IsotropicScatterer : public Scatterer
{
  public:
    void scatter(Batch& batch) override;
};