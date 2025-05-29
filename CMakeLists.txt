cmake_minimum_required(VERSION 3.10)
project(movefetch)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(movefetch
    main.cpp
    config_parser.cpp
    video_ascii_renderer.cpp
    fastfetch_loader.cpp
)

target_link_libraries(movefetch ${OpenCV_LIBS})