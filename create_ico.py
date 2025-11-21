import sys
import struct

def png_to_ico(png_path, ico_path):
    with open(png_path, 'rb') as f:
        png_data = f.read()

    # ICO Header
    # Reserved (2 bytes), Type (2 bytes, 1=ICO), Count (2 bytes)
    header = struct.pack('<HHH', 0, 1, 1)

    # Image Entry
    # Width (1), Height (1), Colors (1), Reserved (1), Planes (2), BPP (2), Size (4), Offset (4)
    # Width/Height 0 means 256
    width = 0
    height = 0
    colors = 0
    reserved = 0
    planes = 1
    bpp = 32
    size = len(png_data)
    offset = 22 # 6 (header) + 16 (entry)

    entry = struct.pack('<BBBBHHII', width, height, colors, reserved, planes, bpp, size, offset)

    with open(ico_path, 'wb') as f:
        f.write(header)
        f.write(entry)
        f.write(png_data)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python3 create_ico.py <input.png> <output.ico>")
        sys.exit(1)
    
    png_to_ico(sys.argv[1], sys.argv[2])
