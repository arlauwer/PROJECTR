#pragma once

#include "Config.hpp"
#include <fstream>
#include <sstream>

class TextInput
{
  public:
    template <typename... Args>
        requires(requires {                                                                    // all Args require:
            typename Args::value_type;                                                         // ::value_type
            std::declval<std::istringstream&>() >> std::declval<typename Args::value_type&>(); // ::operator>>
        } && ...)
    static void read(const std::string& path, Args&... args)
    {
        // Resolve relative to cwd at runtime
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file at " + path);

        string line;
        while (std::getline(file, line))
        {
            if (isComment(line))
                continue;
            std::istringstream ss(line);

            // add value to each arg
            auto push = [&](auto& vec)
            {
                typename std::decay_t<decltype(vec)>::value_type value; // type inside vec
                if (ss >> value)
                    vec.push_back(value);
                else
                    throw std::runtime_error("Requested more columns than available");
            };

            (push(args), ...);
        }
    }

  private:
    static inline bool isComment(const string& line)
    {
        auto start = line.find_first_not_of(" \t\r\n");
        return (line.empty() || start == string::npos || line[start] == '#');
    }
};