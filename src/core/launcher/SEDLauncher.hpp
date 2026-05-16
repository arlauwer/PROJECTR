#pragma once

#include "Launcher.hpp"
#include "SED.hpp"

class SEDLauncher : public Launcher
{
  public:
    SEDLauncher(const string& filepath);

    void launch(Batch& batch) override;

    Capabilities provides() const override;

  private:
    SED _sed;
};