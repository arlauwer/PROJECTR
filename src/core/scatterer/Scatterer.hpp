#pragma once

#include "../batch/Batch.hpp"
#include "../batch/Bitmask.hpp"

class Scatterer
{
  public:
    enum class Capabilities : std::uint32_t
    {
        None = 0,

        Target = 1u << 1,

        All = Target,
    };

    static string to_string(Scatterer::Capabilities field);

    virtual ~Scatterer() = default;

    virtual void launch(Batch& batch) = 0;

    virtual void scatter(Batch& batch) = 0;

    virtual Capabilities provides() const = 0;
};

// enable bitmasking for Scatterer::Capabilities
template <>
inline constexpr bool enable_bitmask<Scatterer::Capabilities> = true;