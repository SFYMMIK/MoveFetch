#pragma once
#include <string>

struct MovefetchConfig {
    std::string path;
    int fps = 12;
    int width = 80;
    int height = 40;
    std::string ascii_chars = "@%#*+=-:. ";
    bool color = true;
};

bool load_config(const std::string& filename, MovefetchConfig& config);