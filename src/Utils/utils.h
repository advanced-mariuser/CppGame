#pragma once

#include <map>
#include <SFML/Graphics.hpp>

namespace utils {
    std::map<std::string, std::map<std::string, std::string>> ReadConfigFile(const std::string& filename);
}