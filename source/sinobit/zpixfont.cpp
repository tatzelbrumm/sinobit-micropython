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
#include "zpixfont.h"

uint8_t zpixfont_char_dwidth(const zpixfont_character_t* character) {
  // Grab the pixel width of this character from the bottom 4 bits of the
  // last data byte.
  return character->data[15] & 0xF;
}

const zpixfont_character_t* zpixfont_find_character(uint16_t encoding) {
  // Find the specified character in the font and return a pointer to it,
  // or null if the character wasn't found.
  // Simple linear search.
  // TODO: Make this a binary search since we know the font array is sorted!
  for (int i=0; i<zpixfont.count; ++i) {
    if (zpixfont.characters[i].encoding == encoding) {
      return &zpixfont.characters[i];
    }
  }
  return NULL;
}

}
