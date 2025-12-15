# depth2ply

这是一个用 C++ 编写的简单工具，将一组深度图（16-bit 或 32-bit）自动降噪并转换为 PLY 点云文件。

依赖
- CMake
- OpenCV (>=3.4)

构建
```bash
mkdir build && cd build
cmake ..
make -j
```

用法
```bash
./depth2ply --input /path/to/depths --output /path/to/out --fx 525 --fy 525 --cx 319.5 --cy 239.5 --scale 1000
```

常用参数
- `--input` 深度图目录（PNG/TIFF/EXR）
- `--output` 输出 PLY 文件目录
- `--fx/--fy/--cx/--cy` 相机内参
- `--scale` 深度缩放（深度单位到米，例如 1000 表示深度以毫米存储）
- `--rgb` 可选 RGB 图像目录（同名文件将被配对）
- `--median` 中值滤波核大小（默认 3）
- `--bd` 双边滤波直径（默认 5）
- `--bsc` 双边滤波色彩 sigma（默认 10）
- `--bsp` 双边滤波空间 sigma（默认 10）

说明
- 本项目在将深度图转换空间坐标时使用: x=(u-cx)*z/fx, y=(v-cy)*z/fy
- 输出为 ASCII PLY，可在 Meshlab、CloudCompare 中打开
