#include "Scatterer.hpp"

string Scatterer::to_string(Scatterer::Capabilities field)
{
    using Capabilities = Scatterer::Capabilities;

    string str;

    if ((field & Capabilities::Target) != Capabilities::None)
        str += "Target|";

    if (str.empty())
        return "None";

    str.pop_back();
    return str;
}