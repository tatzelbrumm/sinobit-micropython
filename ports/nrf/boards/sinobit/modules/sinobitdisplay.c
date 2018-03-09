/**
 *
 * @copyright (c) Christoph Maier
 * @author Christoph Maier.
 * based on work by Tony DiCola
 * Licensed under the Apache License 2.0
 *

 */
#include "mphalport.h"
#include "genhdr/pins.h"
#include "hal_gpio.h"
#include "hal_spi.h"
#include "py/mpprint.h"
#include "py/mphal.h"
#include "py/obj.h"
#include "sinobitdisplay.h"

// From the schematic the sino:bit display is driven by a Holtek HT1632C chip
// connected to the nRF51 with these pins:
// - HT1632C DATA/MOSI - nRF51 P0.21/40 MOSI
// - HT1632C WR/SCK - nRF51 P0.23/42 SCK
// - HT1632C RD/MISO - nRF51 P0.22/41 MISO
// - HT1632C CS - nRF51 P0.16/22
//
// LED matrix is multiplexed with:
// - Rows (common/ground): HTC1632C COM0 to COM11 (12 rows)
// - Columns (anodes): HTC1632C ROW0 to ROW11 (12 rows)
//
// HT1632C needs to be configured with:
// - N-MOS 24 ROW x 16 COL mode

/******************Instructions**********************/
#define SYS_DIS 0x00                //Turn off system shock
#define SYS_EN  0x01                //Turn on  system shock
#define LED_OFF 0x02                //Turn off LED display
#define LED_ON  0x03                //Turn on LED display
#define BLINK_OFF   0x08            //Close blink
#define BLINK_ON    0x09            //Open blink
#define SLAVE_MODE  0x10            //Slave mode
#define RC_MASTER_MODE  0x18        //Use internal clock
#define COM_OPTION  0x24            //
#define PWM_CONTROL 0xA0            //PWM Brightness Control
#define PWM_DUTY    0xAF            //PWM maximum brigntness
/****************I/O definition**********************/

#define HT_CS 16
#define HT_DATA 21
#define HT_RD 22
#define HT_WR 23

static NRF_GPIO_Type *gpiobase = (NRF_GPIO_Type *)NRF_GPIO_BASE;

static inline void CSon(void) { gpiobase->OUTCLR = 1 << HT_CS; }
static inline void CSoff(void) { gpiobase->OUTSET = 1 << HT_CS; }
static inline void RDon(void) { gpiobase->OUTCLR = 1 << HT_RD; }
static inline void RDoff(void) { gpiobase->OUTSET = 1 << HT_RD; }
static inline void WRon(void) { gpiobase->OUTCLR = 1 << HT_WR; }
static inline void WRoff(void) { gpiobase->OUTSET = 1 << HT_WR; }
static inline void DATA0(void) { gpiobase->OUTCLR = 1 << HT_DATA; }
static inline void DATA1(void) { gpiobase->OUTSET = 1 << HT_DATA; }
static inline void RDdata(void) { gpiobase->DIRCLR = 1 << HT_DATA; }
static inline void WRdata(void) { gpiobase->DIRSET = 1 << HT_DATA; }

static uint8_t com[12] = {
    0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x28, 0x2C
};

uint16_t framebuffer[12] = {0};


static void HT1632C_Write(uint8_t Data, uint8_t cnt)      //MCU writes the data to ht1632c, and the high position is in front
{
    uint8_t i;
    for (i = 0; i < cnt; i++) {
        WRon();
        if (Data & 0x80) {
            DATA1();
        } else {
            DATA0();
        }
        Data <<= 1;
        WRoff();
    }
}

static void HT1632C_Write_CMD(uint8_t cmd) //MCU writes commands to ht1632c
{
    WRdata();
    CSon();
    HT1632C_Write(0x80, 3);
    HT1632C_Write(cmd, 9);
    CSoff();
}

static void HT1632C_Write_DAT(uint8_t Addr, const uint16_t data[], uint8_t num)
{
    WRdata();
    CSon();
    HT1632C_Write(0xa0, 3);
    HT1632C_Write(Addr << 1, 7);

    uint16_t d = data[num];
    for (uint8_t i = 0; i < 12; i++) {
        WRon();
        if (d & 0x8000) {
            DATA1();
        } else {
            DATA0();
        }
        d <<= 1;
        WRoff();
    }
    CSoff();
}

static void HT1632C_clr(void)  //Clear function
{
    uint8_t i;
    WRdata();
    CSon();
    HT1632C_Write(0xa0, 3);
    HT1632C_Write(0x00, 7);
    for (i = 0; i < 48; i++) {
        HT1632C_Write(0, 8);
    }
    CSoff();
}

static void HT1632C_Init(void) //HT1632C Init Function
{
    gpiobase->OUTSET = 0x00E10000;
    gpiobase->PIN_CNF[HT_CS] = 0x1;
    gpiobase->PIN_CNF[HT_RD] = 0x1;
    gpiobase->PIN_CNF[HT_WR] = 0x1;
    gpiobase->PIN_CNF[HT_DATA] = 0x1;
    gpiobase->OUTSET = 0x00E10000;
    HT1632C_Write_CMD(SYS_DIS);         //Close the HT1632C internal clock
    HT1632C_Write_CMD(COM_OPTION);      //Select HT1632C work mode
    HT1632C_Write_CMD(RC_MASTER_MODE);  //Select internal clock
    HT1632C_Write_CMD(SYS_EN);          //Open the HT1632C internal clock
    HT1632C_Write_CMD(PWM_DUTY);        //Init the PWM Brightness
    HT1632C_Write_CMD(BLINK_OFF);       //Close blink
    HT1632C_Write_CMD(LED_ON);          // Turn on LED display
}

/*
static uint16_t HT1632C_Read_DATA(uint8_t Addr)
{
    WRdata();
    CSon();
    HT1632C_Write(0xc0, 3);
    HT1632C_Write(Addr << 1, 7);
    RDdata();
    uint16_t datum = 0;
    for (uint8_t i = 0; i < 12; i++) {
        RDon();
        datum <<= 1;
        datum |= (gpiobase->IN & (1 << HT_DATA)) >> (HT_DATA-4);
        RDoff();
    }
    CSoff();
    return datum;
}
*/

static void HT1632C_Write_Pattern(const uint16_t pattern[])
{
    for (int col = 0; col < 12; col++) {
        HT1632C_Write_DAT(com[col], pattern, col);
    }
}

/*
static void HT1632C_Read_Pattern(uint16_t pattern[])
{
    for (int col = 0; col < 12; col++) {
        pattern[col] = HT1632C_Read_DATA(com[col]);
    }
}
*/

// Set brightness to a 4-bit value from 0 (low) to 15 (max).
static void ht1632_brightness(uint8_t brightness) {
    // Clamp brightness value to 0-15.
    // Send brightness command with specified 4 bit value.
    HT1632C_Write_CMD(PWM_CONTROL | (15 > brightness ? brightness : 15));
}

// Set a pixel at the specified x, y position to the provided value.
// True turns on a pixel and false turns it off.
void framebuffer_set(uint8_t x, uint8_t y, bool value) {
    // Ignore values that are out of bounds of the 12x12 portion of display.
    if ((x < 12) && (y < 12)) {
        uint16_t *column = framebuffer+y;
        uint16_t mask = 0x8000 >> x;
        if (value)
            *column |= mask;
        else
            *column &= ~mask;
    }
}

// Get the value of the pixel at the provided x, y position.
// Returns true if set and false if not set (or an invalid position).
bool framebuffer_get(uint8_t x, uint8_t y) {
    bool pixel = 0;
    // Ignore values that are out of bounds of the 12x12 portion of display.
    if ((x < 12) && (y < 12)) {
        pixel = !(!(framebuffer[y] & (0x8000 >> x)));
    }
    return pixel;
}

// Write out the framebuffer data to the display.
void framebuffer_write() {
    HT1632C_Write_Pattern(framebuffer);
}

// Set the entire framebuffer with the provided value (true is on, false is off).
void framebuffer_fill(bool value) {
    // Go through each byte in the buffer and set or unset it appropriately.
    uint16_t datum = 0;
    if (value) datum = ~datum;
    for (int i = 0; i < 12; ++i) {
        framebuffer[i] = datum;
    }
}

// Initialize the display hardware.
void sinobitdisplay_init0() {
    HT1632C_Init();
    // Clear the screen.
    HT1632C_clr();
    framebuffer_fill(false);
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
    //text_draw_string(mp_obj_get_int(x), mp_obj_get_int(y), string);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_3(sinobitdisplay_text_obj, sinobit_display_text);

STATIC mp_obj_t sinobit_display_text_width(mp_obj_t string) {
    //return MP_OBJ_NEW_SMALL_INT(text_width(string));
    return MP_OBJ_NEW_SMALL_INT(0);
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
    .globals = (mp_obj_dict_t*)&sinobitdisplay_module_globals,
};
