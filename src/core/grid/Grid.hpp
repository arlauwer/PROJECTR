#pragma once

#include "../batch/Batch.hpp"

class Grid
{
  public:
    virtual ~Grid() = default;

    virtual unique_ptr<Batch> createBatch(size_t size) = 0;

    virtual void initialize(Batch& batch) = 0;
    virtual void propagate(Batch& batch)  = 0;
};