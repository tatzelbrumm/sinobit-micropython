# Bare-bones BDF format font parsing code.
# Author: Tony DiCola


class Character:
    """Instance of a character from a BDF font.  These are created automatically
    as the font is parsed.  The following members are available once the font
    is loaded:
     - name: The string name assigned to this character.
     - encoding: The Unicode encoding / code point number for this character.
     - dwidth: A 2-tuple of the pixel width, height to advance after rendering
               this character.
     - bbox_size: A 2-tuple of the width, height in pixels of the bounding box
                  that encloses the pixels of this character.
     - bbox_offset: The x, y offset from the lower left corner to start
                    drawing the character.
     - bitmap: A list of booleans that indicate the rendered bitmap data of this
               character.  Use the get_pixel function to read from this bitmap
               at a given location.
    """

    # Define the maximum width and height in pixels (as a 2-tuple) of characters.
    # This is used to set the size of the data buffered used for rendering
    # characters.
    max_size = (0, 0)

    def __init__(self, name, infile):
        self.name = name
        self.parse(infile)

    def parse(self, infile):
        # Parse a character from the BDF font (something enclosed by STARTCHAR
        # and ENDCHAR lines).
        for line in infile:
            parts = line.split()
            if len(parts) == 0:
                continue
            if parts[0] == 'ENCODING':
                # Encoding has the unicode encoding/code page # for this char.
                self.encoding = int(parts[1])
            elif parts[0] == 'DWIDTH':
                # Dwidth is the device pixel width and height to advance after
                # drawing this character.  We need to remember the x / width
                # as it varies for characters thin vs. thick.
                self.dwidth = (int(parts[1]), int(parts[2]))
            elif parts[0] == 'BBX':
                # Bounding box in pixels, specified by width & height then
                # offset x and y from lower left origin.
                self.bbox_size = (int(parts[1]), int(parts[2]))
                self.bbox_offset = (int(parts[3]), int(parts[4]))
            elif parts[0] == 'BITMAP':
                # Parse the following lines as bitmap data.  There will be a
                # row for each pixel in the bounding box height, and a bit
                # for each pixel in the bounding box width (rounded up to next
                # byte boundary and padded with zeros on the right).  We use
                # the bounding box of the character to render it into a grid
                # of boolean values that simulate a framebuffer of the rendered
                # character.  This is useful for later processing which can
                # serialize the rendered character back into a packed binary
                # format.
                max_width, max_height = self.max_size
                self.bitmap = [False] * (max_width * max_height)
                # Figure out how many rows of bitmap data and how many bytes
                # on each row using the bounding box width and height.
                rows = self.bbox_size[1]
                bytes_per_row = (self.bbox_size[0] + 7) // 8  # Round up to next
                                                              # byte boundary.
                # Calculate the upper left corner starting position of the
                # character.  The font specifies the offset from the lower left
                # so we do some math to offset and back up to the top left.
                start_y = (max_height-1) - self.bbox_offset[1] - self.bbox_size[1]
                start_x = 0 + self.bbox_offset[0]
                # Go through each row and read the bytes to set the appropriate
                # pixels in the rendered font.
                for r in range(rows):
                    y = start_y + r
                    x = start_x
                    line = infile.readline().strip()
                    for i in range(0, len(line), 2):
                        byte_val = int(line[i:i+2], 16)
                        for j in range(7, -1, -1):
                            self._set_pixel(x, y, byte_val & (1 << j) > 0)
                            x += 1
            elif parts[0] == 'ENDCHAR':
                return

    @property
    def _stride(self):
        # How many pixels are in a character bitmap row.
        return self.max_size[0]

    def _set_pixel(self, x, y, val):
        # Set a pixel at the specified x, y position to the provided boolean.
        # True if the pixel is set and false if not.
        if 0 <= x < self.max_size[0] and 0 <= y < self.max_size[1]:
            self.bitmap[y*self._stride+x] = val


    def get_pixel(self, x, y):
        """Retrieve a pixel from the rendered char at the specified x, y
        position.  This is a boolean value that's true if the pixel is set and
        false if not set or outside the max_size of the character.
        """
        if 0 <= x < self.max_size[0] and 0 <= y < self.max_size[1]:
            return self.bitmap[y*self._stride+x]
        else:
            return False


class BDFFont:
    """Create an instance of a BDF font from the specified input file.  The
    file should be open for reading and will be read to load all the BDF font
    characters.  The loaded characters are stored in the characters instance
    and are all instances of the Character class.

    Be sure to set the Character.max_size class-level variable before parsing
    a font.  This 2-tuple controls the width and height of each character's
    rendered bitmap data.
    """

    def __init__(self, infile):
        self.characters = []
        self.parse(infile)

    def parse(self, infile):
        # Parse a BDF font file and load all the characters found within.
        for line in infile:
            parts = line.split()
            if len(parts) == 0:
                continue
            if parts[0] == 'STARTCHAR':
                self.characters.append(Character(parts[1], infile))
