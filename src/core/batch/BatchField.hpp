#pragma once

#include "Config.hpp"
#include <cstdint>

enum class BatchField : std::uint32_t
{
    None = 0,

    Position   = 1u << 0,
    Direction  = 1u << 1,
    Luminosity = 1u << 2,
    Wavelength = 1u << 3,

};

constexpr BatchField operator|(BatchField a, BatchField b)
{
    return static_cast<BatchField>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
}

constexpr BatchField operator&(BatchField a, BatchField b)
{
    return static_cast<BatchField>(static_cast<std::uint32_t>(a) & static_cast<std::uint32_t>(b));
}

constexpr BatchField operator~(BatchField a)
{
    return static_cast<BatchField>(~static_cast<std::uint32_t>(a));
}

constexpr BatchField& operator|=(BatchField& a, BatchField b)
{
    a = a | b;
    return a;
}

constexpr BatchField& operator&=(BatchField& a, BatchField b)
{
    a = a & b;
    return a;
}

constexpr string to_string(BatchField field)
{
    string str;

    if ((field & BatchField::Position) != BatchField::None)
        str += "Position|";
    if ((field & BatchField::Direction) != BatchField::None)
        str += "Direction|";
    if ((field & BatchField::Luminosity) != BatchField::None)
        str += "Luminosity|";
    if ((field & BatchField::Wavelength) != BatchField::None)
        str += "Wavelength|";

    if (str.empty())
        return "None";
    str.pop_back();

    return str;
}