#
# Create the hardcoded bitmap structure definitions.
# Documentation for this system is in README.

import os

data_file_name = "bitmap_definitions"

def bitmap_definition(name, horizontal, vertical, bits, centre):
    bm_format_string = """\
Bitmap {}_bm = {{
    .horizontal = {},
    .vertical = {},
    .centre = {{{}, {}}},
    .bits = {{{}}}
}};
"""
    if len(bits) < 32:
        for i in range(32 - len(bits)):
            bits.append("0")
    bit_string = ",".join(["0b" + "".join(bits[i:i+8]) for i in range(0, 32, 8)])
    return bm_format_string.format(name, horizontal, vertical, centre[0], centre[1], bit_string)

def animation_definition(name, bitmap_names):
    animation_format_string = """\
Animation {} = {{
    .speed = 1,
    .frames = {},
    .bitmaps = {{{}}}
}};
"""
    formatted_names = ",".join(["&animation_" + bm_name + "_bm" for bm_name in bitmap_names])
    return animation_format_string.format(name, len(bitmap_names), formatted_names)

# Find the relevant files and directories
bitmap_files = []
animation_directories = []
for filename in os.listdir("bitmaps"):
    if filename.endswith(".bm"):
        bitmap_files.append((filename[:-3], open("bitmaps/" + filename, "r")))
    if filename.startswith("animation") and os.path.isdir("bitmaps/" + filename):
        animation_directories.append(filename)
        for animation_filename in os.listdir("bitmaps/" + filename):
            bitmap_files.append(("animation_" + animation_filename[:-3], open("bitmaps/" + filename + "/" + animation_filename, "r")))

# Create bitmap definitions
bitmap_definitions = []
for name, bm in bitmap_files:
    lines = [line.strip() for line in bm.readlines()][::-1]
    bits = []
    centre = (0, 0)
    for j, line in enumerate(lines):
        for i, entry in enumerate(line):
            if entry == '_':
                bits.append("0")
            elif entry == 'X':
                bits.append("1")
            elif entry == 'C':
                bits.append("0")
                centre = (i, j)
            elif entry == 'O':
                bits.append("1")
                centre = (i, j)
    bitmap_definitions.append(bitmap_definition(name, len(lines[0]), len(lines), bits, centre))

# Create animation definitions
animation_definitions = []
for animation_dir in animation_directories:
    names = sorted([filename[:-3] for filename in os.listdir(f"bitmaps/{animation_dir}")])
    animation_definitions.append(animation_definition(animation_dir, names))

# Write the definitions to the file and echo it
data_file = open(data_file_name, "w+")
for definition in bitmap_definitions + animation_definitions:
    data_file.write(definition)
    print(definition)
    data_file.write("\n")

