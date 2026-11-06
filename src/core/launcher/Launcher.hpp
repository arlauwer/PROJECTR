#pragma once

#include "core/batch/Batch.hpp"
#include "SED.hpp"

class Launcher
{
  public:
    Launcher(const SED& sed);

    virtual ~Launcher() = default;

    virtual void launch(Batch& batch) = 0;

  protected:
    SED _sed;
};
