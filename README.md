# Movefetch 🎥🐧

Movefetch is a fast and stylish animated terminal utility that replaces the typical Neofetch logo with an ASCII-animated video — all while showing real-time system information using Fastfetch.

## ✨ Features

- Convert any video into an ASCII animation (color or monochrome)
- Display system info (CPU, GPU, RAM, etc.) using `fastfetch`
- Automatically places system info below the animation
- Supports ANSI color rendering
- Smart config initialization on first run
- Frame caching for fast playback

## 📦 Requirements

- C++17 or later
- [Fastfetch](https://github.com/fastfetch-cli/fastfetch)
- OpenCV (`libopencv-dev`)
- FFmpeg (for video decoding via OpenCV backend)

Install dependencies on Debian/Ubuntu:
```bash
sudo apt install libopencv-dev ffmpeg fastfetch
```
## 🛠️ Building
- remember you need to have root access to move it to /usr/local/bin.
```bash
cmake CMakeLists.txt && make && sudo mv movefetch /usr/local/bin/
```