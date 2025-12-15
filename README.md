# depth2ply

[English Documentation](README.md) | [中文文档](README.zh-Hans.md)

Small C++ tool to denoise depth images and convert them into ASCII PLY point clouds.

## Features
- Supports 16-bit (mm) and 32-bit depth images.
- Simple denoising pipeline: median blur followed by bilateral filter.
- Optional per-pixel RGB color when matching RGB images are provided.

## Dependencies
- CMake (>= 3.10)
- OpenCV (installed on your system)

## Build
```bash
mkdir -p build && cd build
# If OpenCV is installed via Homebrew, you may need to point CMake to the OpenCV config:
cmake -DOpenCV_DIR="$(brew --prefix opencv)/lib/cmake/opencv4" ..
make -j
```

## Run (example)
```bash
./build/depth2ply --input test_data/depths --rgb test_data/rgb --output out \
	--fx 525 --fy 525 --cx 159.5 --cy 119.5 --scale 1000
```

## Common command-line options
- `--input` : Path to depth images (PNG/TIFF/EXR supported).
- `--output` : Output directory for PLY files.
- `--fx/--fy/--cx/--cy` : Camera intrinsics.
- `--scale` : Depth scale factor (e.g. `1000` if depth is stored in millimeters).
- `--rgb` : Optional path to RGB images. The tool will try to pair by filename, and will also attempt to replace a leading `depth_` prefix with `rgb_` if the direct match is missing.
- `--median` : Median filter kernel size (default `3`).
- `--bd` : Bilateral filter diameter (default `5`).
- `--bsc` : Bilateral filter sigmaColor (default `10`).
- `--bsp` : Bilateral filter sigmaSpace (default `10`).

## Output details
- Point coordinates are computed with: x = (u - cx) * z / fx, y = (v - cy) * z / fy, where z is in meters.
- Output format: ASCII PLY (can be opened in MeshLab or CloudCompare).

## Test data
- Use `tools/generate_test_images.py` to generate sample depth and RGB images into `test_data/depths` and `test_data/rgb`.

## VSCode / IntelliSense tips
- If the editor reports missing standard headers or OpenCV headers, ensure CMake has been run and `build/compile_commands.json` is generated:
	```bash
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DOpenCV_DIR="$(brew --prefix opencv)/lib/cmake/opencv4" build
	```
- Then in VSCode run: `Developer: Reload Window` or `C/C++: Reset IntelliSense Database`.

## FAQ / Troubleshooting
- "Could not find OpenCVConfig.cmake": on macOS install OpenCV via Homebrew and point CMake to `$(brew --prefix opencv)/lib/cmake/opencv4`.
- `bilateralFilter` assertions: the code converts depth to `CV_32F` before filtering; if you still see errors, please paste the terminal output.