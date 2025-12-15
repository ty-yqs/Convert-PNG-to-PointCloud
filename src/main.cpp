#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "../include/depth_to_cloud.h"
#include "../include/ply_writer.h"

namespace fs = std::filesystem;

static void printUsage() {
    std::cout << "Usage: depth2ply --input <depth_dir> --output <out_dir> --fx F --fy F --cx C --cy C [options]\n";
    std::cout << "Options:\n";
    std::cout << "  --scale S       depth scale (e.g. 1000 for mm->m). default=1000\n";
    std::cout << "  --rgb <dir>     optional rgb images directory (matching filenames)\n";
}

int main(int argc, char** argv) {
    if (argc < 9) {
        printUsage();
        return 1;
    }

    std::string inputDir, outputDir, rgbDir;
    Intrinsics K{0,0,0,0};
    float scale = 1000.0f;
    DenoiseParams denoise;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--input" && i+1<argc) inputDir = argv[++i];
        else if (a == "--output" && i+1<argc) outputDir = argv[++i];
        else if (a == "--rgb" && i+1<argc) rgbDir = argv[++i];
        else if (a == "--fx" && i+1<argc) K.fx = std::stof(argv[++i]);
        else if (a == "--fy" && i+1<argc) K.fy = std::stof(argv[++i]);
        else if (a == "--cx" && i+1<argc) K.cx = std::stof(argv[++i]);
        else if (a == "--cy" && i+1<argc) K.cy = std::stof(argv[++i]);
        else if (a == "--scale" && i+1<argc) scale = std::stof(argv[++i]);
        else if (a == "--median" && i+1<argc) denoise.medianKernel = std::stoi(argv[++i]);
        else if (a == "--bd" && i+1<argc) denoise.bilateralDiameter = std::stoi(argv[++i]);
        else if (a == "--bsc" && i+1<argc) denoise.bilateralSigmaColor = std::stod(argv[++i]);
        else if (a == "--bsp" && i+1<argc) denoise.bilateralSigmaSpace = std::stod(argv[++i]);
    }

    if (inputDir.empty() || outputDir.empty() || K.fx==0 || K.fy==0) {
        printUsage();
        return 1;
    }

    fs::create_directories(outputDir);

    for (auto &p : fs::directory_iterator(inputDir)) {
        if (!p.is_regular_file()) continue;
        std::string ext = p.path().extension().string();
        if (ext != ".png" && ext != ".tiff" && ext != ".tif" && ext != ".exr") continue;

        std::string filename = p.path().filename().string();
        std::cout << "Processing " << filename << "\n";

        cv::Mat depth = cv::imread(p.path().string(), cv::IMREAD_UNCHANGED);
        if (depth.empty()) {
            std::cerr << "Failed to load " << p.path() << "\n";
            continue;
        }

        cv::Mat rgb;
        if (!rgbDir.empty()) {
            std::string rgbPath = (fs::path(rgbDir) / filename).string();
            rgb = cv::imread(rgbPath, cv::IMREAD_COLOR);
            if (rgb.empty()) {
                // try alternate naming: replace leading "depth_" with "rgb_" if present
                std::string stem = p.path().stem().string();
                std::string ext = p.path().extension().string();
                std::string altStem = stem;
                if (stem.rfind("depth_", 0) == 0) {
                    altStem = std::string("rgb_") + stem.substr(6);
                }
                std::string altName = altStem + ext;
                std::string altPath = (fs::path(rgbDir) / altName).string();
                rgb = cv::imread(altPath, cv::IMREAD_COLOR);
                if (rgb.empty()) {
                    std::cerr << "Warning: rgb missing for " << filename << "\n";
                } else {
                    std::cout << "Using alternate rgb " << altName << "\n";
                }
            }
        }

        std::vector<cv::Vec3f> points;
        std::vector<cv::Vec3b> colors;
        depthToPointCloud(depth, rgb, K, scale, denoise, points, colors);

        std::string outPath = (fs::path(outputDir) / (p.path().stem().string() + ".ply")).string();
        bool ok = writePly(outPath, points, colors);
        if (!ok) std::cerr << "Failed to write " << outPath << "\n";
    }

    std::cout << "All done. PLY files are in " << outputDir << "\n";
    return 0;
}
