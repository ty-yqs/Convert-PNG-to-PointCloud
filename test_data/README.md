Test data folder

This folder contains synthetic depth and RGB images for quick testing.

To generate images (requires Python):

```bash
pip install numpy pillow
python3 tools/generate_test_images.py
```

Depth images are 16-bit PNG (units: millimeters). RGB images are 8-bit PNG.

Example run (after building the project):

```bash
./build/depth2ply --input test_data/depths --rgb test_data/rgb --output out --fx 525 --fy 525 --cx 159.5 --cy 119.5 --scale 1000
```
