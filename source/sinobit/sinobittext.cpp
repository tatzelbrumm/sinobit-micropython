/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Tony DiCola
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
extern "C" {

#include <cstddef>

#include "py/objstr.h"
#include "py/unicode.h"

#include "sinobittext.h"
#include "sinobitdisplay.h"
#include "zpixfont.h"


static void draw_char(int x0, int y0, const zpixfont_character_t* character) {
  // Draw the specified character at an x, y location on the framebuffer.
  for (int y=0; y<11; ++y) {
    for (int x=0; x<11; ++x) {
      uint8_t byte_index = (y*11+x) / 8;
      uint8_t byte_offset = 7-((y*11+x) % 8);
      if ((character->data[byte_index] & (1 << byte_offset)) > 0) {
        // Draw pixels for the character.  Note the character x/y axes are
        // swapped to draw on the sinobit display correctly.
        framebuffer_set(y0+y, x0+x, true);
      }
    }
  }
}

void text_draw_char(int x0, int y0, uint16_t encoding) {
  // Find the character and display it on the framebuffer.
  const zpixfont_character_t* character = zpixfont_find_character(encoding);
  if (character == NULL) {
    return;
  }
  draw_char(x0, y0, character);
}

void text_draw_string(int x0, int y0, mp_obj_t string) {
  // Go through each character in the string and draw it starting at the
  // specified x, y location.
  int x = x0;
  GET_STR_DATA_LEN(string, data, len);
  const uint8_t* s = data;
  for (unsigned int i=0; i<unichar_charlen((const char *)data, len); ++i) {
    unichar encoding = utf8_get_char(s);
    // Find the character.
    const zpixfont_character_t* character = zpixfont_find_character(encoding);
    if (character != NULL) {
      // Draw the character and advance to draw next character after the
      // width of this one.
      draw_char(x, y0, character);
      x += zpixfont_char_dwidth(character);
    }
    else {
      // TODO: Handle character not found by rendering a box or similar
      // missing character.  For now just skip 11 pixels.
      x += 11;
    }
    s = utf8_next_char(s);
  }
}

uint16_t text_width(mp_obj_t string) {
  // Determine the pixel width of the specified string and return it.
  uint16_t width = 0;
  GET_STR_DATA_LEN(string, data, len);
  const uint8_t* s = data;
  for (unsigned int i=0; i<unichar_charlen((const char *)data, len); ++i) {
    unichar encoding = utf8_get_char(s);
    // Find the character.
    const zpixfont_character_t* character = zpixfont_find_character(encoding);
    if (character != NULL) {
      width += zpixfont_char_dwidth(character);
    }
    else {
      // TODO: Handle character not found by rendering a box or similar
      // missing character.  For now just skip 11 pixels.
      width += 11;
    }
    s = utf8_next_char(s);
  }
  return width;
}

}
