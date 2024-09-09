from PIL import Image
import numpy as np

path_to_image = input();
rgba_image = Image.open(path_to_image)
rgb_image = rgba_image.convert('RGB')

pix_val = list(rgb_image.getdata())

width = rgb_image.width
height = rgb_image.height


def converRGBToHex(color):
    hex = '0x{:02x}{:02x}{:02x}'.format(*color)
    return '0x0000' if hex == '0xffffff' else '0xFFFF'

hex_list = list(map(converRGBToHex, pix_val))

result = np.array(hex_list).reshape(height, width)

for row in result:
    str = ",".join(row.tolist())
    print(str + ',')
