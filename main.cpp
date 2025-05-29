
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <vector>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <pwd.h>
#include "fastfetch_loader.h"
#include "video_ascii_renderer.h"
#include "config_parser.h"

using namespace std;

string get_default_config_path() {
    const char* config_home = getenv("XDG_CONFIG_HOME");
    if (config_home) {
        return string(config_home) + "/movefetch/movefetch.conf";
    } else {
        const char* home = getenv("HOME");
        if (!home) home = getpwuid(getuid())->pw_dir;
        return string(home) + "/.config/movefetch/movefetch.conf";
    }
}

void create_config_interactively(const string& config_path, MovefetchConfig& config) {
    cout << "\nNo config file found. Let's create one!" << endl;

    cout << "Enter path to video: ";
    getline(cin, config.path);

    cout << "Enter FPS (default 12): ";
    string fps_input;
    getline(cin, fps_input);
    config.fps = fps_input.empty() ? 12 : stoi(fps_input);

    cout << "Enter frame width (default 80): ";
    string width_input;
    getline(cin, width_input);
    config.width = width_input.empty() ? 80 : stoi(width_input);

    cout << "Enter frame height (default 40): ";
    string height_input;
    getline(cin, height_input);
    config.height = height_input.empty() ? 40 : stoi(height_input);

    cout << "Use color output? (true/false, default true): ";
    string color_input;
    getline(cin, color_input);
    config.color = color_input.empty() ? true : (color_input == "true");

    config.ascii_chars = "@%#*+=-:. ";

    filesystem::create_directories(filesystem::path(config_path).parent_path());
    ofstream out(config_path);
    out << "path=" << config.path << "\n";
    out << "fps=" << config.fps << "\n";
    out << "width=" << config.width << "\n";
    out << "height=" << config.height << "\n";
    out << "color=" << (config.color ? "true" : "false") << "\n";
    out << "ascii=" << config.ascii_chars << "\n";
    out.close();

    cout << "\nConfig saved to " << config_path << "\nStarting conversion..." << endl;
}

void convert_video(const MovefetchConfig& config) {
    vector<string> ascii_frames;
    if (!render_video_to_ascii(config, ascii_frames)) {
        cerr << "Failed to render video." << endl;
        return;
    }

    filesystem::create_directories("frames");
    for (size_t i = 0; i < ascii_frames.size(); ++i) {
        ofstream out("frames/frame_" + to_string(i + 1) + ".txt");
        out << ascii_frames[i];
    }

    ofstream meta("frames/info.conf");
    meta << "frame_count=" << ascii_frames.size() << "\n";
    meta << "fps=" << config.fps << "\n";
    meta << "color=" << (config.color ? "true" : "false") << "\n";

    cout << "Conversion complete. Saved " << ascii_frames.size() << " frames to ./frames/." << endl;
}

int main(int argc, char** argv) {
    MovefetchConfig config;
    string config_path = get_default_config_path();

    if (!filesystem::exists(config_path)) {
        string user_choice;
        cout << "Config file not found at " << config_path << ". Create one now? (y/n): ";
        getline(cin, user_choice);
        if (user_choice == "y" || user_choice == "Y") {
            create_config_interactively(config_path, config);
            convert_video(config);
            return 0;
        } else {
            cerr << "Cannot continue without a config file." << endl;
            return 1;
        }
    }

    if (!load_config(config_path, config)) {
        cerr << "Failed to load config from: " << config_path << endl;
        return 1;
    }

    if (argc > 1 && string(argv[1]) == "--convert") {
        convert_video(config);
        return 0;
    }

    // Load Fastfetch output once
    string sysinfo = get_fastfetch_output();
    vector<string> sysinfo_lines;
    istringstream sysinfo_stream(sysinfo);
    string sysline;
    while (getline(sysinfo_stream, sysline)) {
        sysinfo_lines.push_back(sysline);
    }

    // Read frames from disk
    ifstream meta("frames/info.conf");
    if (!meta.is_open()) {
        cerr << "Missing frames/info.conf. Run with --convert first." << endl;
        return 2;
    }

    int frame_count = 0, fps = 12;
    string line;
    while (getline(meta, line)) {
        if (line.find("frame_count=") == 0) frame_count = stoi(line.substr(12));
        if (line.find("fps=") == 0) fps = stoi(line.substr(4));
    }

    vector<string> ascii_frames;
    for (int i = 0; i < frame_count; ++i) {
        ifstream in("frames/frame_" + to_string(i + 1) + ".txt");
        stringstream ss;
        ss << in.rdbuf();
        ascii_frames.push_back(ss.str());
    }

    const int delay_ms = 1000 / fps;
    for (const auto& frame : ascii_frames) {
        system("clear");
        istringstream frame_stream(frame);
        string frame_line;
        size_t frame_line_idx = 0;

        while (getline(frame_stream, frame_line)) {
            if (frame_line_idx < sysinfo_lines.size()) {
                cout << frame_line << "  " << sysinfo_lines[frame_line_idx] << "\n";
            } else {
                cout << frame_line << "\n";
            }
            ++frame_line_idx;
        }
        this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }

    return 0;
}