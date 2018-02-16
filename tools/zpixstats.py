# Script to generate stats on a BDF format font.  This is a quick and dirty
# analysis that helps undestand the structure and type of data needed to store
# character glyphs in a packed binary format.


INPUT_FILE = './zpix/src/Zpix.bdf'


class Character:

    def __init__(self):
        self.encoding = -1
        self.dwidth_x = -1
        self.dwidth_y = -1
        self.bbox_width = -1
        self.bbox_height = -1
        self.bbox_x = 0
        self.bbox_y = 0


def parse_character(infile):
    # Parse a character from the BDF font (something enclosed by STARTCHAR
    # and ENDCHAR lines).  Returns the character state as a Character instance.
    char = Character()
    for line in infile:
        parts = line.split()
        if len(parts) == 0:
            continue
        if parts[0] == 'ENCODING':
            char.encoding = int(parts[1])
        elif parts[0] == 'DWIDTH':
            char.dwidth_x = int(parts[1])
            char.dwidth_y = int(parts[2])
        elif parts[0] == 'BBX':
            char.bbox_width = int(parts[1])
            char.bbox_height = int(parts[2])
            char.bbox_x = int(parts[3])
            char.bbox_y = int(parts[4])
        elif parts[0] == 'ENDCHAR':
            return char

# Open the input file and parse all the characters.
with open(INPUT_FILE, 'r') as infile:
    chars = []
    for line in infile:
        parts = line.split()
        if len(parts) == 0:
            continue
        if parts[0] == 'STARTCHAR':
            chars.append(parse_character(infile))

# Print analytics from the characters.
# Total characters.
print(f'Loaded {len(chars)} chars')
# Min and max encoding values.
min_encoding = chars[0].encoding
max_encoding = chars[0].encoding
# Distribution of dwidth x and y values.
dwidth_x_dist = {}
dwidth_y_dist = {}
# Distribution of bbox values.
bbox_width_dist = {}
bbox_height_dist = {}
bbox_x_dist = {}
bbox_y_dist = {}
# Process all the characters to update min, max, and distributions.
for c in chars:
    min_encoding = min(c.encoding, min_encoding)
    max_encoding = max(c.encoding, max_encoding)
    dwidth_x_dist[c.dwidth_x] = dwidth_x_dist.get(c.dwidth_x, 0) + 1
    dwidth_y_dist[c.dwidth_y] = dwidth_y_dist.get(c.dwidth_y, 0) + 1
    bbox_width_dist[c.bbox_width] = bbox_width_dist.get(c.bbox_width, 0) + 1
    bbox_height_dist[c.bbox_height] = bbox_height_dist.get(c.bbox_height, 0) + 1
    bbox_x_dist[c.bbox_x] = bbox_x_dist.get(c.bbox_x, 0) + 1
    bbox_y_dist[c.bbox_y] = bbox_y_dist.get(c.bbox_y, 0) + 1
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
