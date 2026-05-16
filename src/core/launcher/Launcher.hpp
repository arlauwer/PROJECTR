#pragma once

#include "../batch/Batch.hpp"
#include "../batch/Bitmask.hpp"

class Launcher
{
  public:
    enum class Capabilities : std::uint32_t
    {
        None = 0,

        Position   = 1u << 0,
        Direction  = 1u << 1,
        Luminosity = 1u << 2,
        Wavelength = 1u << 3,

        All = Position | Direction | Luminosity | Wavelength,
    };

    static string to_string(Launcher::Capabilities field);

    virtual ~Launcher() = default;

    virtual void launch(Batch& batch) = 0;

    virtual Capabilities provides() const = 0;
};

// enable bitmasking for Launcher::Capabilities
template <>
inline constexpr bool enable_bitmask<Launcher::Capabilities> = true;