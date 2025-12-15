#include "../include/depth_to_cloud.h"

void depthToPointCloud(const cv::Mat &depthImage, const cv::Mat &rgbImage,
                       const Intrinsics &K, float depthScale,
                       const DenoiseParams &denoise,
                       std::vector<cv::Vec3f> &outPoints,
                       std::vector<cv::Vec3b> &outColors) {
    CV_Assert(!depthImage.empty());

    cv::Mat depthF;
    // support 16U (mm) and 32F (meters)
    if (depthImage.type() == CV_16U) {
        depthImage.convertTo(depthF, CV_32F, 1.0f / depthScale);
    } else if (depthImage.type() == CV_32F) {
        depthF = depthImage.clone();
    } else {
        depthImage.convertTo(depthF, CV_32F);
    }

    // Denoise: median then bilateral (on float depth)
    cv::Mat denoised = depthF;
    if (denoise.medianKernel > 1)
        cv::medianBlur(depthF, denoised, denoise.medianKernel);
    if (denoise.bilateralDiameter > 0) {
        cv::Mat tmp;
        cv::bilateralFilter(denoised, tmp, denoise.bilateralDiameter,
                            denoise.bilateralSigmaColor,
                            denoise.bilateralSigmaSpace);
        denoised = tmp;
    }

    outPoints.clear();
    outColors.clear();
    outPoints.reserve(100000);
    outColors.reserve(100000);

    const int rows = denoised.rows;
    const int cols = denoised.cols;

    bool hasColor = !rgbImage.empty() && rgbImage.size() == denoised.size();

    for (int v = 0; v < rows; ++v) {
        const float* dptr = denoised.ptr<float>(v);
        const cv::Vec3b* cptr = hasColor ? rgbImage.ptr<cv::Vec3b>(v) : nullptr;
        for (int u = 0; u < cols; ++u) {
            float z = dptr[u];
            if (z <= 0.0f || !std::isfinite(z)) continue;
            float x = (u - K.cx) * z / K.fx;
            float y = (v - K.cy) * z / K.fy;
            outPoints.emplace_back(x, y, z);
            if (hasColor) outColors.push_back(cptr[u]);
        }
    }
}
