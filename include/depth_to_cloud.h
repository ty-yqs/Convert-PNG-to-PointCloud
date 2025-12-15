#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

struct Intrinsics {
    float fx, fy, cx, cy;
};

struct DenoiseParams {
    int medianKernel = 3; // odd
    int bilateralDiameter = 5;
    double bilateralSigmaColor = 10.0;
    double bilateralSigmaSpace = 10.0;
};

// Convert a single depth image (and optional rgb) into points and colors.
void depthToPointCloud(const cv::Mat &depthImage, const cv::Mat &rgbImage,
                       const Intrinsics &K, float depthScale,
                       const DenoiseParams &denoise,
                       std::vector<cv::Vec3f> &outPoints,
                       std::vector<cv::Vec3b> &outColors);
