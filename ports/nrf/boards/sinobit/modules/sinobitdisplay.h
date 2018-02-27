#ifndef SINOBIT_DISPLAY_H
#define SINOBIT_DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

void framebuffer_set(uint8_t x, uint8_t y, bool value);

bool framebuffer_get(uint8_t x, uint8_t y);

void framebuffer_write();

void framebuffer_fill(bool value);

void sinobitdisplay_init0();

#endif
