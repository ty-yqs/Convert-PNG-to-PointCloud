# depth2ply

这是一个小型的 C++ 工具，用于对深度图进行去噪并将其转换为 ASCII PLY 点云。

## 功能
- 支持 16-bit（毫米）和 32-bit 深度图。
- 简单去噪流程：中值滤波 + 双边滤波。
- 可选的每像素 RGB 颜色（当提供匹配的 RGB 图像时）。

## 依赖
- CMake (>= 3.10)
- OpenCV（需在系统上安装）

## 构建
```bash
mkdir -p build && cd build
# 如果通过 Homebrew 安装了 OpenCV，可能需要指向 OpenCV 的 CMake 配置：
cmake -DOpenCV_DIR="$(brew --prefix opencv)/lib/cmake/opencv4" ..
make -j
```

## 运行示例
```bash
./build/depth2ply --input test_data/depths --rgb test_data/rgb --output out \
  --fx 525 --fy 525 --cx 159.5 --cy 119.5 --scale 1000
```

## 常用参数
- `--input` : 深度图目录（支持 PNG/TIFF/EXR）。
- `--output` : 输出 PLY 文件目录。
- `--fx/--fy/--cx/--cy` : 相机内参。
- `--scale` : 深度缩放（例如深度以毫米存储时使用 `1000`）。
- `--rgb` : 可选 RGB 图像目录。工具会尝试按文件名配对，若找不到则尝试将 `depth_` 前缀替换为 `rgb_`。
- `--median` : 中值滤波核大小（默认 `3`）。
- `--bd` : 双边滤波直径（默认 `5`）。
- `--bsc` : 双边滤波 sigmaColor（默认 `10`）。
- `--bsp` : 双边滤波 sigmaSpace（默认 `10`）。

## 输出说明
- 坐标计算公式：x = (u - cx) * z / fx, y = (v - cy) * z / fy，z 单位为米。
- 输出为 ASCII PLY，可使用 MeshLab 或 CloudCompare 打开。

## 测试数据
- 使用 `tools/generate_test_images.py` 可以生成示例深度与 RGB 图像到 `test_data/depths` 和 `test_data/rgb`。

## VSCode / IntelliSense 提示
- 如果编辑器提示找不到标准头或 OpenCV 头，先确保已运行 CMake 并生成 `build/compile_commands.json`：
  ```bash
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DOpenCV_DIR="$(brew --prefix opencv)/lib/cmake/opencv4" build
  ```
- 然后在 VSCode 中运行：`Developer: Reload Window` 或 `C/C++: Reset IntelliSense Database`。

## 常见问题
- 找不到 `OpenCVConfig.cmake`：在 macOS 上常用 Homebrew 安装 OpenCV，并在 CMake 时指定 `-DOpenCV_DIR=$(brew --prefix opencv)/lib/cmake/opencv4`。
- 报 `bilateralFilter` 相关错误：代码会先将深度转换为 `CV_32F`；如果仍然报错，请把终端输出贴上来。