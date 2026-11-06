#pragma once

#include "Scatterer.hpp"

class IsotropicScatterer : public Scatterer
{
  public:
    void launch(Batch& batch) override;

    void scatter(Batch& batch) override;

    Capabilities provides() const override;
};