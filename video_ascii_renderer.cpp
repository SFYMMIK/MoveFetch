#include "video_ascii_renderer.h"
#include <opencv2/opencv.hpp>
#include <sstream>

bool render_video_to_ascii(const MovefetchConfig& config, std::vector<std::string>& frames) {
    cv::VideoCapture cap(config.path);
    if (!cap.isOpened()) return false;

    int frame_interval = static_cast<int>(1000.0 / config.fps);
    double frame_time = 0.0;
    double frame_rate = cap.get(cv::CAP_PROP_FPS);

    while (true) {
        cap.set(cv::CAP_PROP_POS_MSEC, frame_time);
        cv::Mat frame;
        if (!cap.read(frame)) break;

        cv::Mat resized, gray;
        cv::resize(frame, resized, cv::Size(config.width, config.height));
        if (!config.color) {
            cv::cvtColor(resized, gray, cv::COLOR_BGR2GRAY);
        }

        std::ostringstream ascii_frame;
        for (int y = 0; y < resized.rows; ++y) {
            for (int x = 0; x < resized.cols; ++x) {
                char c;
                if (config.color) {
                    cv::Vec3b color = resized.at<cv::Vec3b>(y, x);
                    int brightness = (color[2] + color[1] + color[0]) / 3;
                    c = config.ascii_chars[brightness * config.ascii_chars.size() / 256];
                    ascii_frame << "\033[38;2;" << (int)color[2] << ";" << (int)color[1] << ";" << (int)color[0] << "m" << c;
                } else {
                    uchar val = gray.at<uchar>(y, x);
                    c = config.ascii_chars[val * config.ascii_chars.size() / 256];
                    ascii_frame << c;
                }
            }
            ascii_frame << "\033[0m\n";
        }

        frames.push_back(ascii_frame.str());
        frame_time += frame_interval;
    }

    return true;
}