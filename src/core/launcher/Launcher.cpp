#include "Launcher.hpp"

string Launcher::to_string(Launcher::Capabilities field)
{
    using Capabilities = Launcher::Capabilities;

    string str;

    if ((field & Capabilities::Position) != Capabilities::None)
        str += "Position|";
    if ((field & Capabilities::Direction) != Capabilities::None)
        str += "Direction|";
    if ((field & Capabilities::Luminosity) != Capabilities::None)
        str += "Luminosity|";
    if ((field & Capabilities::Wavelength) != Capabilities::None)
        str += "Wavelength|";

    if (str.empty())
        return "None";

    str.pop_back();
    return str;
}
