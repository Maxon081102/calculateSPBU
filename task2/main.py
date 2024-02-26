import argparse
import numpy as np

from PIL import Image

from rotate import rotate_img

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Rotate image')
    parser.add_argument('--input', type=str, help='Input image for rotation')
    parser.add_argument('--output', type=str, help='Output image')
    args = parser.parse_args()
    
    img_source = args.input
    img_for_save = args.output
    
    img = Image.open(img_source)
    img_rotated = rotate_img(img)
    img_rotated.save(img_for_save)