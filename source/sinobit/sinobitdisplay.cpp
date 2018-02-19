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

#include "py/mpprint.h"
#include "py/mphal.h"
#include "py/obj.h"
#include "spi_api.h"
#include "modmicrobit.h"
#include "modsinobit.h"
#include "microbitpin.h"
#include "microbitobj.h"
#include "nrf_gpio.h"
#include "sinobitdisplay.h"
#include "sinobittext.h"
#include "zpixfont.h"

// From the schematic the sino:bit display is driven by a Holtek HT1632C chip
// connected to the nRF51 with these pins:
// - HT1632C DATA/MOSI - nRF51 P0.21/40 MOSI
// - HT1632C WR/SCK - nRF51 P0.23/42 SCK
// - HT1632C RD/MISO - nRF51 P0.22/41 MISO
// - HT1632C CS - nRF51 P0.16/22
//
// LED matrix is charlieplexed with:
// - Rows (common/ground): HTC1632C COM0 to COM11 (12 rows)
// - Columns (anodes): HTC1632C ROW0 to ROW11 (12 rows)
//
// HT1632C needs to be configured with:
// - N-MOS 24 ROW x 16 COL mode

// HT1632 command and other defines.
#define HT1632_WRITE         0x2800
#define HT1632_COMMAND       0x0400
#define HT1632_SYS_DIS       0x00
#define HT1632_SYS_EN        0x01
#define HT1632_LED_OFF       0x02
#define HT1632_LED_ON        0x03
#define HT1632_BLINK_OFF     0x08
#define HT1632_BLINK_ON      0x09
#define HT1632_RC_MASTER     0x18
#define HT1632_PWM_CONTROL   0xA0
#define HT1632_COMMON_16NMOS 0x24

// Defines to simplify asserting and deasserting the CS line of the display.
#define ASSERT_HT1632_CS { nrf_gpio_pin_clear(microbit_p16_obj.name); }
#define DEASSERT_HT1632_CS { nrf_gpio_pin_set(microbit_p16_obj.name); }

// Reference to the HAL SPI bus.
static spi_t spi;
// The buffer of pixel data.  Each bit represents a pixel at a row & column
// and is directly mapped to the HT1632C display memory.  Although only 12x12
// pixels are used this buffer is padded out to support the full 16x12 pixels
// of data in the display--this greatly simplfies writing out the data to the
// display later (the unused columns are ignored and never set).  We get 24
// bytes because 12 rows * 2 bytes per row (16 pixels).
uint8_t framebuffer[24] = {0};


// Send a command to the HT1632 chip.  Specify the 8 bits of the command
// and it will be sent with the proper header and other state.
static void ht1632_command(uint8_t command) {
    // Construct full command as a 16-bit value.  This is actually not correct
    // because the hardware expects a 12-bit word, but the HAL / hardware
    // doesn't seem to support _anything_ besides 8-bit words (bytes).  Luckily
    // it appears the HT1632C ignores trailing bits so we can send a 16-bit
    // value (two bytes) with the 12-bit command shifted up to the most
    // significant bits.
    uint16_t full_command = HT1632_COMMAND | command;
    full_command <<= 5;  // Shift up so command bits are at top.
    // Assert CS and send command bytes.
    ASSERT_HT1632_CS;
    spi_master_write(&spi, (full_command >> 8) & 0xFF);
    spi_master_write(&spi, full_command & 0xFF);
    DEASSERT_HT1632_CS;
}

// Write 4 bits of display data to the provided 7-bit address.
static void ht1632_write(uint8_t address, uint8_t data) {
    // Like with writing a command we have to work with the 8-bit word
    // constraint of the hardware.  Luckily the HT1632C seems to ignore
    // trailing bits and we can send the 14 bit display data in a 16 bit word
    // (two bytes).
    // First chop off any unused bits from the address and data.
    address &= 0x7F;  // Address is 7 bits.
    data &= 0x0F;     // Data is 4 bits.
    // Construct write command.
    uint16_t write = HT1632_WRITE | ((uint16_t)address << 4) | data;
    write <<= 2;  // Shift up so command bits are at top.
    // Assert CS and send write command bytes.
    ASSERT_HT1632_CS;
    spi_master_write(&spi, (write >> 8) & 0xFF);
    spi_master_write(&spi, write & 0xFF);
    DEASSERT_HT1632_CS;
}

// Set brightness to a 4-bit value from 0 (low) to 15 (max).
static void ht1632_brightness(uint8_t brightness) {
    // Clamp brightness value to 0-15.
    if (brightness > 15) {
        brightness = 15;
    }
    // Send brightness command with specified 4 bit value.
    ht1632_command(HT1632_PWM_CONTROL | (brightness & 0xF));
}

// Set a pixel at the specified x, y position to the provided value.
// True turns on a pixel and false turns it off.
void framebuffer_set(uint8_t x, uint8_t y, bool value) {
    // Ignore values that are out of bounds of the 12x12 portion of display.
    if ((x > 11) || (y > 11)) {
      return;
    }
    // Calculate bit position of this pixel within framebuffer.
    uint8_t bit = y*16+x;
    uint8_t byte_pos = bit / 8;
    uint8_t byte_offset = bit % 8;
    // Invert position within each 4 bit nibble to match order of bits for
    // HT1632 display memory.
    if (byte_offset < 4) {
        // 0 1 2 3 -> 3 2 1 0
        byte_offset = 3 - byte_offset;
    }
    else {
        // 4 5 6 7 -> 7 6 5 4
        byte_offset = 11 - byte_offset;
    }
    // Turn on or off the appropriate bit depending on the value.
    if (value) {
        framebuffer[byte_pos] |= (1 << byte_offset);
    }
    else {
        framebuffer[byte_pos] &= ~(1 << byte_offset);
    }
}

// Get the value of the pixel at the provided x, y position.
// Returns true if set and false if not set (or an invalid position).
bool framebuffer_get(uint8_t x, uint8_t y) {
    // Ignore values that are out of bounds of the 12x12 portion of display.
    if ((x > 11) || (y > 11)) {
      return false;
    }
    // Calculate bit position of this pixel within framebuffer.
    uint8_t bit = y*16+x;
    uint8_t byte_pos = bit / 8;
    uint8_t byte_offset = bit % 8;
    // Invert position within each 4 bit nibble to match order of bits for
    // HT1632 display memory.
    if (byte_offset < 4) {
        // 0 1 2 3 -> 3 2 1 0
        byte_offset = 3 - byte_offset;
    }
    else {
        // 4 5 6 7 -> 7 6 5 4
        byte_offset = 11 - byte_offset;
    }
    // Check if the bit at the pixel position is set.
    return (framebuffer[byte_pos] & (1 << byte_offset)) > 0;
}

// Write out the framebuffer data to the display.
void framebuffer_write() {
    // Sadly we can't do a single address and success data writes to efficiently
    // write the framebuffer data in one transaction.  Again the problem is
    // SPI word size and our limit of using 8-bit words.  The display buffer
    // data starts with a 14 bit alignment and then 4-bit words.  As a result
    // we just break down the framebuffer update into individual address writes.
    uint8_t i = 0;
    // Write out all 12 rows of the framebuffer.
    for (int r=0; r<12; r++) {
        // For each row write out the 4 columns, grabbing the appropriate nibble
        // of 4-bit data from the framebuffer.
        uint8_t address = r*4;
        ht1632_write(address,   framebuffer[i]);
        ht1632_write(address+1, framebuffer[i] >> 4);
        i += 1;
        ht1632_write(address+2, framebuffer[i]);
        ht1632_write(address+3, framebuffer[i] >> 4);
        i += 1;
    }
}

// Set the entire framebuffer with the provided value (true is on, false is off).
void framebuffer_fill(bool value) {
    // Go through each byte in the buffer and set or unset it appropriately.
    for (int i=0; i<24; ++i) {
        if (value) {
            framebuffer[i] = 0xFF;
        }
        else {
            framebuffer[i] = 0x00;
        }
    }
}

// Initialize the display hardware.
void sinobit_display_init() {
    // Initialize the SPI bus.
    spi_init(&spi, (PinName)microbit_p15_obj.name,  // MOSI
                   (PinName)microbit_p14_obj.name,  // MISO
                   (PinName)microbit_p13_obj.name,  // SCK
                   NC);                             // CS (CANNOT be set!)
    spi_format(&spi, 8, 0, 0);     // MUST be 8 bits per word!
    spi_frequency(&spi, 1000000);  // 1mhz clock
    // Setup pin 16 CS as output that's high by default.
    microbit_obj_pin_acquire(&microbit_p16_obj,
                             microbit_pin_mode_write_digital);
    nrf_gpio_cfg_output(microbit_p16_obj.name);
    DEASSERT_HT1632_CS;
    // Initialize the display.
    ht1632_command(HT1632_SYS_EN);             // Turn on oscillator
    ht1632_command(HT1632_LED_ON);             // Turn on LED duty cyle gen
    ht1632_command(HT1632_BLINK_OFF);          // Turn off blink
    ht1632_command(HT1632_RC_MASTER);          // RC master mode
    ht1632_command(HT1632_COMMON_16NMOS);      // 24 x 16 NMOS open drain
    ht1632_command(HT1632_PWM_CONTROL | 0xF);  // Full PWM duty cycle
    // Clear the screen.
    framebuffer_fill(false);
    framebuffer_write();
}

// These are the functions exposed by the module to Python code.  See the
// locals table macro at the bottom for how these map to QSTR names.
STATIC mp_obj_t sinobit_display_set_pixel(mp_obj_t x, mp_obj_t y, mp_obj_t c) {
    framebuffer_set(mp_obj_get_int(x), mp_obj_get_int(y), mp_obj_is_true(c));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_3(sinobitdisplay_set_pixel_obj, sinobit_display_set_pixel);

STATIC mp_obj_t sinobit_display_get_pixel(mp_obj_t x, mp_obj_t y) {
    if (framebuffer_get(mp_obj_get_int(x), mp_obj_get_int(y))) {
        return mp_const_true;
    }
    else {
        return mp_const_false;
    }
}
MP_DEFINE_CONST_FUN_OBJ_2(sinobitdisplay_get_pixel_obj, sinobit_display_get_pixel);

STATIC mp_obj_t sinobit_display_write() {
    framebuffer_write();
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(sinobitdisplay_write_obj, sinobit_display_write);

STATIC mp_obj_t sinobit_display_fill(mp_obj_t c) {
    framebuffer_fill(mp_obj_is_true(c));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(sinobitdisplay_fill_obj, sinobit_display_fill);

STATIC mp_obj_t sinobit_display_clear() {
    framebuffer_fill(false);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(sinobitdisplay_clear_obj, sinobit_display_clear);

STATIC mp_obj_t sinobit_display_brightness(mp_obj_t brightness) {
    ht1632_brightness(mp_obj_get_int(brightness));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(sinobitdisplay_brightness_obj, sinobit_display_brightness);

STATIC mp_obj_t sinobit_display_text(mp_obj_t x, mp_obj_t y, mp_obj_t string) {
    text_draw_string(mp_obj_get_int(x), mp_obj_get_int(y), string);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_3(sinobitdisplay_text_obj, sinobit_display_text);

STATIC mp_obj_t sinobit_display_text_width(mp_obj_t string) {
    return MP_OBJ_NEW_SMALL_INT(text_width(string));
}
MP_DEFINE_CONST_FUN_OBJ_1(sinobitdisplay_text_width_obj, sinobit_display_text_width);

STATIC const mp_map_elem_t sinobitdisplay_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_display) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_set_pixel), (mp_obj_t)&sinobitdisplay_set_pixel_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_get_pixel), (mp_obj_t)&sinobitdisplay_get_pixel_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_write), (mp_obj_t)&sinobitdisplay_write_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_fill), (mp_obj_t)&sinobitdisplay_fill_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_clear), (mp_obj_t)&sinobitdisplay_clear_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_brightness), (mp_obj_t)&sinobitdisplay_brightness_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_text), (mp_obj_t)&sinobitdisplay_text_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_text_width), (mp_obj_t)&sinobitdisplay_text_width_obj },
};

STATIC MP_DEFINE_CONST_DICT(sinobitdisplay_module_globals, sinobitdisplay_module_globals_table);

const mp_obj_module_t sinobitdisplay_module = {
    .base = { &mp_type_module },
    .name = MP_QSTR_display,
    .globals = (mp_obj_dict_t*)&sinobitdisplay_module_globals,
};

}
