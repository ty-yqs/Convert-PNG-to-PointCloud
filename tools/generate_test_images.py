#!/usr/bin/env python3
"""
Generate synthetic test depth (16-bit PNG, millimeters) and RGB images.
Creates files under test_data/depths and test_data/rgb.
Usage:
  pip install numpy pillow
  python3 tools/generate_test_images.py
"""
import os
import numpy as np
from PIL import Image

OUT_DEPTH = os.path.join(os.path.dirname(__file__), '..', 'test_data', 'depths')
OUT_RGB = os.path.join(os.path.dirname(__file__), '..', 'test_data', 'rgb')
OUT_DEPTH = os.path.normpath(OUT_DEPTH)
OUT_RGB = os.path.normpath(OUT_RGB)
os.makedirs(OUT_DEPTH, exist_ok=True)
os.makedirs(OUT_RGB, exist_ok=True)

H, W = 240, 320
for i in range(3):
    # depth in meters, simple gradient + offset
    X = np.tile(np.linspace(0.2, 1.5, W, dtype=np.float32), (H,1))
    Y = np.tile(np.linspace(0.0, 0.8, H, dtype=np.float32)[:,None], (1,W))
    depth = ((i+1)*0.5 + X*0.5 + Y*0.2).astype(np.float32) # meters
    depth_mm = (depth * 1000.0).astype(np.uint16)
    if i == 1:
        # add some missing (zero) region
        depth_mm[60:120, 100:180] = 0
    depth_path = os.path.join(OUT_DEPTH, f'depth_{i:02d}.png')
    Image.fromarray(depth_mm).save(depth_path)

    # rgb image: simple gradients
    rgb = np.zeros((H,W,3), dtype=np.uint8)
    rgb[...,0] = (np.clip((X - X.min())/(X.max()-X.min()), 0, 1) * 255).astype(np.uint8)
    rgb[...,1] = (np.clip((Y - Y.min())/(Y.max()-Y.min()), 0, 1) * 255).astype(np.uint8)
    rgb[...,2] = np.full((H,W), (i+1)*80, dtype=np.uint8)
    rgb_path = os.path.join(OUT_RGB, f'rgb_{i:02d}.png')
    Image.fromarray(rgb).save(rgb_path)

print('Generated 3 depth and 3 rgb images:')
print('  depth ->', OUT_DEPTH)
print('  rgb   ->', OUT_RGB)
print('\nRun your tool with:')
print('  ./build/depth2ply --input test_data/depths --rgb test_data/rgb --output out --fx 525 --fy 525 --cx 159.5 --cy 119.5 --scale 1000')
