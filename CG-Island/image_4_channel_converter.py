import sys
from PIL import Image
import os

def add_alpha_channel(input_path):
    # Open the image
    img = Image.open(input_path)

    # Create a new alpha channel
    alpha_channel = Image.new('L', img.size, 255)  # 255 means fully opaque

    # Add the alpha channel to the original image
    img.putalpha(alpha_channel)

    output_path = input_path.replace(".", ".4channel.")

    img.save(output_path)

    # Check the number of channels in the modified image
    modified_img = Image.open(output_path)
    num_channels = len(modified_img.getbands())
    if num_channels == 4:
        print(f"The modified image has {num_channels} channels.")
        img.save(input_path)
    else:
        print("failed convertion")

    os.remove(output_path)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py input_image_path")
        sys.exit(1)

    input_image_path = sys.argv[1]
    add_alpha_channel(input_image_path)
