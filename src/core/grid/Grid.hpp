#pragma once

#include "../batch/Batch.hpp"
#include "Config.hpp"
#include "RadiationField.hpp"

class Grid
{
  public:
    virtual ~Grid() = default;

    virtual unique_ptr<Batch> createBatch(size_t size) = 0;

    virtual void initialize(Batch& batch) = 0;
    virtual void propagate(Batch& batch)  = 0;

    vector<real>& kappa()
    {
        return _kappa;
    }

    vector<real>& albedo()
    {
        return _albedo;
    }

    RadiationField& radiationField()
    {
        return *_radField;
    }

  protected:
    vector<real>                  _kappa;
    vector<real>                  _albedo;
    std::optional<RadiationField> _radField;
};