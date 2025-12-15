#pragma once

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

// Write point cloud to ASCII PLY. If colors is empty, write only vertices.
bool writePly(const std::string &filename,
              const std::vector<cv::Vec3f> &points,
              const std::vector<cv::Vec3b> &colors);
