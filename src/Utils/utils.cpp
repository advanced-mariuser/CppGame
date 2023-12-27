#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::map<std::string, std::map<std::string, std::string>> ReadConfigFile(const std::string& filename) {
    std::map<std::string, std::map<std::string, std::string>> configData;
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return configData;
    }

    std::string line, currentSection;

    while (std::getline(infile, line)) {
        if (!line.empty() && line[0] == '[' && line[line.size() - 1] == ']') {
            currentSection = line.substr(1, line.size() - 2);
        } else if (!currentSection.empty()) {
            size_t equalsPos = line.find('=');
            if (equalsPos != std::string::npos) {
                std::string key = line.substr(0, equalsPos);
                std::string value = line.substr(equalsPos + 1);
                configData[currentSection][key] = value;
            }
        }
    }

    infile.close();
    return configData;
}
