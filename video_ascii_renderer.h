#pragma once
#include <vector>
#include <string>
#include "config_parser.h"

bool render_video_to_ascii(const MovefetchConfig& config, std::vector<std::string>& frames);