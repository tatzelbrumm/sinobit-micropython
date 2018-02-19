# Define the packed binary struct representation of the font and each character.
# This attempts to be as space efficient as possible in order to include as
# many characters as possible.  In addition the list of characters is stored in
# sorted order for fast lookup (binary search).  The character list will never
# change at run time so no need for more complex data structures that support
# fast insertion, deletion, etc--everything is precomputed ahead of time so only
# total space and lookup speed are concerns.
# Author: Tony DiCola
#
# Font structure:
# - uint8_t version: A number that indicates the 'version' of the font format.
#                    This is useful for forwards an backwards compatibility.
#                    For this font the version is always set to 1.  Later
#                    versions might increment the value and have a completely
#                    different structure.
# - uint16_t count:  The number of characters in the font.
# - character[] characters:  An array of character data.  Each character
#                            instance is a fixed size struct defined below.
#                            This array is _sorted_ by ascending character
#                            encoding order to allow fast space-efficient
#                            lookup with binary search.
#
# Character structure:
# - uint16_t encoding: The unicode encoding / code point of the character.
# - uint8_t[16] data: Array of 16 bytes that tightly packs an 11x11 grid of
#                     character bitmap data.  For alignment purposes there will
#                     be 7 extra bits added (since 11x11 = 121 bits which is
#                     not evenly divisible by 8 and pads up to 128 bits).  The
#                     four low bits of this last 7 bit value are used to encode
#                     an unsigned 4-bit value (0-15) which is the pixel width
#                     of this character, i.e. how far to the right to move
#                     before adding a row of padding and rendering the next
#                     character.  The three top bits of this 7-bit value are
#                     currently unused.  Here's a visual representation of the
#                     11x11 pixel grid with spaces indicating the byte bounds:
#                      00000000  000      0 = character pixel (1 = set)
#                      00000  000000      . = unused
#                      00 00000000 0      w = 4-bit unsigned width
#                      0000000  0000
#                      0000  0000000
#                      0 00000000 00
#                      000000  00000
#                      000  00000000
#                      00000000  000
#                      00000  000000
#                      00 00000000 0
#                      ...wwww
# For convenience these are the algorithms to convert from character pixel
# position to byte and offset within the byte:
# - Given x, y find byte and offset inside byte (from right, i.e. shift left):
#   byte_index = (y*11+x) // 8
#   byte_offset = 7-((y*11+x) % 8)
# - Given byte index and offset (from right) find x, y:
#   y = (byte_index*8+(7-byte_offset)) // 11
#   x = (byte_index*8+(7-byte_offset)) % 11
import bdf


class ZpixFontCharacter:

    def __init__(self, character):
        self._character = character

    @property
    def name(self):
        """The friendly name of the character (from the BDF font file).
        """
        return self._character.name

    @property
    def encoding(self):
        """The unicode encoding/code point of the character, a 16-bit value.
        """
        return self._character.encoding

    @property
    def data(self):
        """Serialize the bitmap data of the character to a tightly packed
        16 byte array of bitmap data.  The last byte's 4 low bits also encode
        the pixel width of the character.
        """
        data = bytearray(16)
        # Go through each pixel of the character bitmap and encode it in the
        # bitmap bits.  If the a pixel is set then inject a 1 bit, otherwise
        # keep it as a 0 bit (unset pixel).
        for y in range(11):
            for x in range(11):
                byte_index = (y*11+x) // 8
                byte_offset = 7-((y*11+x) % 8)
                if self._character.get_pixel(x, y):
                    data[byte_index] |= (1 << byte_offset)
        # Add the dwidth as the lowest 4 bits of the entire array.
        data[15] |= (self._character.dwidth[0] & 0xF)
        return data


class ZpixFont:

    def __init__(self, font_bdf='./zpix/src/Zpix.bdf'):
        bdf.Character.max_size = (11, 11)
        with open(font_bdf, 'r') as infile:
            self._zpix = bdf.BDFFont(infile)
        # Build a dictionary of character encoding value to BDF font character.
        # This allows fast lookups of BDF characters.
        self._characters = {}
        for c in self._zpix.characters:
            self._characters[c.encoding] = c

    def get_characters(self, *args):
        """Retrieve instances of ZPixFontCharacters for all the specified
        character encodings.  Each argument can be an individual encoding
        / code point number, or an iterable of size 2 that defines the
        inclusive range of code points to include (like (10, 100) would include
        all code points between 10 and 100, including 10 and 100).  If a
        character is requested (either explicitly or inside a range) and not
        found in the font then it will be dropped and not included.  The
        returned results are sorted in ascending encoding / code point order.
        """
        # Build a sorted set of all the encodings.
        encodings = set()
        for val in args:
            # Handle single numbers.
            if isinstance(val, int):
                encodings.add(val)
            # Handle iterables (lists/tuples)
            elif len(val) == 2:
                encodings.update(range(val[0], val[1]+1))
        # Sort the set of encodings and iterate through it to assemble a list
        # of result characters to return.
        characters = []
        for e in sorted(encodings):
            if e in self._characters:
                characters.append(ZpixFontCharacter(self._characters[e]))
        return characters


if __name__ == '__main__':
    # Simple test case to serialize a few characters and print them as C code.
    font = ZpixFont()
    chars = font.get_characters(33, 34, (35, 40))
    print(f'// Encoded {len(chars)} characters:')
    print(f"""const struct {{
  uint8_t version;
  uint16_t count;
  struct {{
    uint16_t encoding;
    uint8_t data[16];
  }} characters[{len(chars)}];
}} zpixFont = {{
  .version = 1,
  .count = {len(chars)},
  .characters = {{""")
    for c in chars[:-1]:
        data_bytes = ', '.join(map(lambda x: f'0x{x:02X}', c.data))
        print(f"""    {{
      .encoding = {c.encoding},\t// {c.name}
      .data = {{ {data_bytes} }}
    }},""")
    c = chars[-1]
    data_bytes = ', '.join(map(lambda x: f'0x{x:02X}', c.data))
    print(f"""    {{
      .encoding = {c.encoding},\t// {c.name}
      .data = {{ {data_bytes} }}
    }}
  }}
}};""")
