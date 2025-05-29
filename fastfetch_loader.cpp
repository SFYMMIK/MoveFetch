#include "fastfetch_loader.h"
#include <array>
#include <memory>
#include <cstdio>

std::string get_fastfetch_output() {
    std::string result;
    std::array<char, 256> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("fastfetch --logo none", "r"), pclose);
    if (!pipe) return "Fastfetch failed.\n";

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}