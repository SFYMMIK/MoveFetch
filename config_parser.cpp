#include "config_parser.h"
#include <fstream>
#include <sstream>
#include <algorithm>

bool load_config(const std::string& filename, MovefetchConfig& config) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '[' || line[0] == '#') continue;
        auto delim = line.find('=');
        if (delim == std::string::npos) continue;

        std::string key = line.substr(0, delim);
        std::string val = line.substr(delim + 1);

        key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
        val.erase(std::remove_if(val.begin(), val.end(), ::isspace), val.end());

        if (key == "path") config.path = val;
        else if (key == "fps") config.fps = std::stoi(val);
        else if (key == "width") config.width = std::stoi(val);
        else if (key == "height") config.height = std::stoi(val);
        else if (key == "ascii") config.ascii_chars = val;
        else if (key == "color") config.color = (val == "true");
    }
    return true;
}
