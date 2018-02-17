# Script to generate stats on a BDF format font.  This is a quick and dirty
# analysis that helps undestand the structure and type of data needed to store
# character glyphs in a packed binary format.
# Author: Tony DiCola
import bdf


INPUT_FILE          = './zpix/src/Zpix.bdf'  # Font to load.
CHARACTER_MAX_SIZE  = (11, 11)               # Max x, y dimensions of characters
RENDER_CHARACTER    = 36                     # Encoding of character to render
                                             # at end as a test.


def increment(dict_, key):
    # Increment the specified key in a dictionary by one.  Will handle cases
    # where the key doesn't exist and initialize it with 0 (then increment to 1).
    dict_[key] = dict_.get(key, 0) + 1

# Open the input file and parse all the characters.
print(f'Loading {INPUT_FILE}...')
with open(INPUT_FILE, 'r') as infile:
    # Make sure to set max character dimensions before loading font.
    bdf.Character.max_size = CHARACTER_MAX_SIZE
    font = bdf.BDFFont(infile)
print(f'Loaded {len(font.characters)} characters!')

# Calculate some statistics and analytics of the font characters.
# This data is useful to inform the amount of bytes and dimension for packing
# characters into a binary format for inclusion in the firmware.
min_encoding = font.characters[0].encoding  # Min and max encoding values.
max_encoding = font.characters[0].encoding
dwidth_x_dist = {}     # Distribution of dwidth x and y values.
dwidth_y_dist = {}
bbox_width_dist = {}   # Distribution of bbox values.
bbox_height_dist = {}
bbox_x_dist = {}
bbox_y_dist = {}
# 'Oversize' characters that are larger than 11 pixels in a dimension.
oversize_chars = set()
# Process all the characters to update min, max, and distributions.
for c in font.characters:
    min_encoding = min(c.encoding, min_encoding)
    max_encoding = max(c.encoding, max_encoding)
    increment(dwidth_x_dist, c.dwidth[0])
    increment(dwidth_y_dist, c.dwidth[1])
    increment(bbox_width_dist, c.bbox_size[0])
    increment(bbox_height_dist, c.bbox_size[1])
    increment(bbox_x_dist, c.bbox_offset[0])
    increment(bbox_y_dist, c.bbox_offset[1])
    if c.bbox_size[0] > CHARACTER_MAX_SIZE[0] or \
       c.bbox_size[1] > CHARACTER_MAX_SIZE[1]:
        oversize_chars.add(c.encoding)

# Print values found through the analysis above.
print(f'Min encoding: {min_encoding}')
print(f'Max encoding: {max_encoding}')
print('Dwidth X distribution:')
for x, count in sorted(dwidth_x_dist.items()):
    print(f' {x}: {count}')
print('Dwidth Y distribution:')
for x, count in sorted(dwidth_y_dist.items()):
    print(f' {x}: {count}')
print('Bbox width distribution:')
for x, count in sorted(bbox_width_dist.items()):
    print(f' {x}: {count}')
print('Bbox height distribution:')
for x, count in sorted(bbox_height_dist.items()):
    print(f' {x}: {count}')
print('Bbox x distribution:')
for x, count in sorted(bbox_x_dist.items()):
    print(f' {x}: {count}')
print('Bbox y distribution:')
for x, count in sorted(bbox_y_dist.items()):
    print(f' {x}: {count}')
print(f'Oversize character count: {len(oversize_chars)}')
print('Over size characters:')
for c in sorted(oversize_chars):
    # Print the decimal encoding, unicode hex input, and actual character.
    print(f'{c} - U+{c:04x} - {c:c}')

# Find a character and render it, this is useful for testing bitmap rendering
# or viewing a specific character (like one which is oversize).
for c in font.characters:
    if c.encoding == RENDER_CHARACTER:
        break
print(f'Bitmap for character {c.encoding} {c.name}:')
print('.{0}.'.format('-' * CHARACTER_MAX_SIZE[0]))
for y in range(CHARACTER_MAX_SIZE[1]):
    print('|', end='')
    for x in range(CHARACTER_MAX_SIZE[0]):
        if c.get_pixel(x, y):
            print('8', end='')  # Render 8 for a set pixel.
        elif x >= c.dwidth[0]:
            print('.', end='')  # Render . for a pixel past the dwidth of char.
        else:
            print(' ', end='')  # Render space for unset pixel.
    print('|')
print("'{0}'".format('-' * CHARACTER_MAX_SIZE[0]))
