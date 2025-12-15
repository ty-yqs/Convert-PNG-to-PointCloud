#include "../include/ply_writer.h"
#include <fstream>

bool writePly(const std::string &filename,
              const std::vector<cv::Vec3f> &points,
              const std::vector<cv::Vec3b> &colors) {
    if (points.empty()) return false;
    bool hasColor = !colors.empty() && colors.size() == points.size();

    std::ofstream ofs(filename);
    if (!ofs.is_open()) return false;

    ofs << "ply\n";
    ofs << "format ascii 1.0\n";
    ofs << "element vertex " << points.size() << "\n";
    ofs << "property float x\n";
    ofs << "property float y\n";
    ofs << "property float z\n";
    if (hasColor) {
        ofs << "property uchar red\n";
        ofs << "property uchar green\n";
        ofs << "property uchar blue\n";
    }
    ofs << "end_header\n";

    for (size_t i = 0; i < points.size(); ++i) {
        const auto &p = points[i];
        ofs << p[0] << " " << p[1] << " " << p[2];
        if (hasColor) {
            const auto &c = colors[i];
            ofs << " " << int(c[2]) << " " << int(c[1]) << " " << int(c[0]);
        }
        ofs << "\n";
    }

    ofs.close();
    return true;
}
