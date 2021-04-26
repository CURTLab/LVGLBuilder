/*******************************************************************************
 * Size: 12 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_FONT_123ABC_12
#define LV_FONT_123ABC_12 1
#endif

#if LV_FONT_123ABC_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0021 "!" */
    0xf, 0x10, 0xf1, 0xf, 0x10, 0xf1, 0xf, 0x0,
    0xf0, 0x9, 0x0, 0x30, 0xe, 0x10,

    /* U+0022 "\"" */
    0x39, 0x93, 0x39, 0x92, 0x38, 0x91, 0x1, 0x10,

    /* U+0023 "#" */
    0x0, 0xc, 0x7, 0x60, 0x0, 0x1c, 0xa, 0x20,
    0xc, 0xee, 0xef, 0xe3, 0x0, 0x76, 0x1c, 0x0,
    0x0, 0xa3, 0x49, 0x0, 0x4e, 0xfe, 0xee, 0xb0,
    0x0, 0xd0, 0x93, 0x0, 0x2, 0xb0, 0xc1, 0x0,
    0x4, 0x80, 0xd0, 0x0,

    /* U+0024 "$" */
    0x0, 0xd, 0x0, 0x0, 0x6, 0xf6, 0x0, 0xa,
    0xc7, 0xd8, 0x0, 0xf2, 0x3, 0xe0, 0xe, 0x40,
    0x5, 0x0, 0x6f, 0x81, 0x0, 0x0, 0x3a, 0xf5,
    0x0, 0x0, 0x4, 0xe0, 0x4d, 0x0, 0xf, 0x0,
    0xe7, 0x4a, 0xc0, 0x2, 0xaf, 0x91, 0x0, 0x0,
    0xd0, 0x0,

    /* U+0025 "%" */
    0xb, 0xc8, 0x0, 0x0, 0x5, 0x80, 0xc0, 0x28,
    0x0, 0x58, 0xd, 0xb, 0x10, 0x0, 0x9c, 0x65,
    0x70, 0x0, 0x0, 0x1, 0xc0, 0x0, 0x0, 0x0,
    0x94, 0x8c, 0x80, 0x0, 0x3a, 0x2b, 0xa, 0x30,
    0xb, 0x12, 0xa0, 0xa3, 0x0, 0x0, 0xa, 0xca,
    0x0,

    /* U+0026 "&" */
    0x2, 0xcf, 0xb0, 0x0, 0x9, 0x90, 0xb6, 0x0,
    0x9, 0x80, 0xc4, 0x0, 0x3, 0xec, 0x80, 0x0,
    0x4, 0xee, 0x0, 0x30, 0x2e, 0x29, 0xb2, 0xd0,
    0x5b, 0x0, 0xcc, 0x90, 0x2e, 0x10, 0x5f, 0x50,
    0x5, 0xdd, 0xc7, 0xe1,

    /* U+0027 "'" */
    0x67, 0x67, 0x66, 0x0,

    /* U+0028 "(" */
    0x0, 0x2, 0x0, 0x5a, 0x1, 0xd0, 0x8, 0x80,
    0xd, 0x30, 0x1f, 0x0, 0x2e, 0x0, 0x3e, 0x0,
    0x2f, 0x0, 0xf, 0x10, 0xb, 0x50, 0x5, 0xa0,
    0x0, 0xb3, 0x0, 0x19,

    /* U+0029 ")" */
    0x20, 0x0, 0x87, 0x0, 0xd, 0x20, 0x7, 0x90,
    0x2, 0xf0, 0x0, 0xe2, 0x0, 0xc4, 0x0, 0xc4,
    0x0, 0xd3, 0x0, 0xf1, 0x3, 0xd0, 0x9, 0x60,
    0x2c, 0x0, 0x82, 0x0,

    /* U+002A "*" */
    0x0, 0xd0, 0x4, 0x1c, 0x4, 0x7d, 0xfe, 0x90,
    0x6e, 0x80, 0x1d, 0x1d, 0x20, 0x10, 0x10,

    /* U+002B "+" */
    0x0, 0x1b, 0x0, 0x0, 0x2, 0xe0, 0x0, 0x0,
    0x2e, 0x0, 0x8, 0xff, 0xff, 0xf4, 0x12, 0x4e,
    0x22, 0x0, 0x2, 0xe0, 0x0, 0x0, 0x2e, 0x0,
    0x0,

    /* U+002C "," */
    0x4c, 0x5b, 0xa4, 0x0,

    /* U+002D "-" */
    0xbf, 0xf1,

    /* U+002E "." */
    0x4, 0x1, 0xe1,

    /* U+002F "/" */
    0x0, 0x8, 0x60, 0x0, 0xd1, 0x0, 0x3b, 0x0,
    0x9, 0x50, 0x0, 0xd0, 0x0, 0x4a, 0x0, 0xa,
    0x40, 0x0, 0xd0, 0x0, 0x59, 0x0, 0xb, 0x30,
    0x0,

    /* U+0030 "0" */
    0x3, 0xde, 0xb1, 0x0, 0xe5, 0x9, 0xa0, 0x3d,
    0x0, 0x2e, 0x4, 0xc0, 0x0, 0xf0, 0x5c, 0x0,
    0xf, 0x14, 0xc0, 0x0, 0xf0, 0x3e, 0x0, 0x2f,
    0x0, 0xe5, 0x9, 0xa0, 0x3, 0xdf, 0xb1, 0x0,

    /* U+0031 "1" */
    0x5, 0xc4, 0xea, 0xe4, 0x10, 0xd4, 0x0, 0xd4,
    0x0, 0xd4, 0x0, 0xd4, 0x0, 0xd4, 0x0, 0xd4,
    0x0, 0xd4,

    /* U+0032 "2" */
    0x5, 0xdf, 0xb2, 0x2, 0xe3, 0x9, 0xb0, 0x48,
    0x0, 0x3d, 0x0, 0x0, 0x7, 0x90, 0x0, 0x2,
    0xe1, 0x0, 0x1, 0xd4, 0x0, 0x0, 0xc6, 0x0,
    0x0, 0xb8, 0x0, 0x0, 0x4f, 0xff, 0xff, 0x40,

    /* U+0033 "3" */
    0x5, 0xde, 0xb1, 0x2e, 0x20, 0x99, 0x25, 0x0,
    0x4c, 0x0, 0x0, 0xa8, 0x0, 0xbf, 0xd1, 0x0,
    0x0, 0x8a, 0x24, 0x0, 0x2e, 0x3e, 0x20, 0x8b,
    0x6, 0xef, 0xb1,

    /* U+0034 "4" */
    0x0, 0x3, 0xf4, 0x0, 0x0, 0xcf, 0x40, 0x0,
    0x6a, 0xc4, 0x0, 0x1d, 0x1c, 0x40, 0x9, 0x70,
    0xc4, 0x3, 0xd0, 0xc, 0x40, 0xaf, 0xff, 0xff,
    0x70, 0x0, 0xc, 0x40, 0x0, 0x0, 0xc4, 0x0,

    /* U+0035 "5" */
    0x6, 0xff, 0xff, 0x0, 0x88, 0x11, 0x10, 0x9,
    0x60, 0x0, 0x0, 0xbc, 0xec, 0x30, 0x6, 0x62,
    0x8e, 0x0, 0x0, 0x0, 0xe3, 0x7, 0x0, 0xd,
    0x30, 0xd6, 0x5, 0xe0, 0x2, 0xcf, 0xd3, 0x0,

    /* U+0036 "6" */
    0x0, 0x5d, 0xf0, 0x0, 0x5e, 0x40, 0x0, 0xd,
    0x40, 0x0, 0x1, 0xf9, 0xec, 0x20, 0x3f, 0x60,
    0x7d, 0x3, 0xe0, 0x0, 0xf1, 0x1f, 0x0, 0xf,
    0x10, 0xb8, 0x7, 0xd0, 0x1, 0xbf, 0xc2, 0x0,

    /* U+0037 "7" */
    0x8f, 0xff, 0xff, 0x30, 0x0, 0x2, 0xd0, 0x0,
    0x0, 0x97, 0x0, 0x0, 0x1e, 0x0, 0x0, 0x7,
    0x90, 0x0, 0x0, 0xe2, 0x0, 0x0, 0x5b, 0x0,
    0x0, 0xc, 0x50, 0x0, 0x3, 0xe0, 0x0, 0x0,

    /* U+0038 "8" */
    0x3, 0xdf, 0xb1, 0x0, 0xe5, 0x9, 0xa0, 0x1f,
    0x0, 0x3d, 0x0, 0xd5, 0x9, 0x90, 0x4, 0xff,
    0xe1, 0x1, 0xe4, 0x7, 0xb0, 0x4c, 0x0, 0xf,
    0x2, 0xe3, 0x6, 0xd0, 0x5, 0xdf, 0xc2, 0x0,

    /* U+0039 "9" */
    0x5, 0xde, 0x90, 0x1f, 0x30, 0xc7, 0x5b, 0x0,
    0x4c, 0x5b, 0x0, 0x2e, 0x1f, 0x30, 0xae, 0x5,
    0xee, 0x8d, 0x0, 0x0, 0x79, 0x0, 0x15, 0xe2,
    0x3, 0xfc, 0x30,

    /* U+003A ":" */
    0x1e, 0x0, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x40, 0x1e, 0x0,

    /* U+003B ";" */
    0x3d, 0x4, 0x0, 0x0, 0x0, 0x18, 0x3d, 0x79,
    0x50,

    /* U+003C "<" */
    0x0, 0x2, 0x93, 0x2, 0x9f, 0x91, 0x6e, 0x71,
    0x0, 0x4d, 0xa3, 0x0, 0x0, 0x6e, 0xc1, 0x0,
    0x0, 0x62,

    /* U+003D "=" */
    0x1e, 0xee, 0xeb, 0x1, 0x11, 0x11, 0x0, 0x0,
    0x0, 0x1e, 0xee, 0xeb, 0x1, 0x11, 0x11,

    /* U+003E ">" */
    0x3a, 0x20, 0x0, 0x8, 0xea, 0x30, 0x0, 0x6,
    0xd9, 0x0, 0x3a, 0xe6, 0x1c, 0xe7, 0x0, 0x26,
    0x0, 0x0,

    /* U+003F "?" */
    0x9, 0xfe, 0x50, 0x6c, 0x13, 0xf0, 0x11, 0x0,
    0xf1, 0x0, 0x5, 0xc0, 0x0, 0x4e, 0x20, 0x0,
    0xc5, 0x0, 0x0, 0x51, 0x0, 0x0, 0x30, 0x0,
    0x0, 0xc3, 0x0,

    /* U+0040 "@" */
    0x0, 0x6, 0xcd, 0xd9, 0x10, 0x0, 0x9, 0x91,
    0x0, 0x4c, 0x10, 0x5, 0x90, 0x0, 0x0, 0x39,
    0x0, 0xc1, 0x5, 0xdc, 0x30, 0xb0, 0x1c, 0x3,
    0xd0, 0x76, 0x9, 0x13, 0x90, 0x86, 0x9, 0x40,
    0x92, 0x48, 0xb, 0x30, 0xa3, 0xa, 0x13, 0x90,
    0xb4, 0xe, 0x31, 0xb0, 0xc, 0x4, 0xda, 0x5d,
    0xb2, 0x0, 0xb3, 0x0, 0x0, 0x0, 0x0, 0x2,
    0xd4, 0x0, 0x11, 0x0, 0x0, 0x2, 0xac, 0xcc,
    0x30, 0x0,

    /* U+0041 "A" */
    0x0, 0xc, 0x90, 0x0, 0x0, 0x2e, 0xe0, 0x0,
    0x0, 0x79, 0xc4, 0x0, 0x0, 0xd4, 0x7a, 0x0,
    0x3, 0xe0, 0x1f, 0x10, 0x9, 0xa0, 0xc, 0x60,
    0xe, 0xff, 0xff, 0xc0, 0x4e, 0x0, 0x1, 0xf2,
    0xa8, 0x0, 0x0, 0xb7,

    /* U+0042 "B" */
    0xf, 0xff, 0xe8, 0x0, 0xf1, 0x2, 0xd5, 0xf,
    0x10, 0x9, 0x80, 0xf1, 0x2, 0xe4, 0xf, 0xff,
    0xfb, 0x0, 0xf1, 0x1, 0xb8, 0xf, 0x10, 0x5,
    0xc0, 0xf1, 0x1, 0xb9, 0xf, 0xff, 0xea, 0x0,

    /* U+0043 "C" */
    0x0, 0x8e, 0xea, 0x10, 0x9, 0xc2, 0x9, 0xc0,
    0x1f, 0x20, 0x0, 0xe2, 0x3e, 0x0, 0x0, 0x0,
    0x4d, 0x0, 0x0, 0x0, 0x3e, 0x0, 0x0, 0x0,
    0x1f, 0x10, 0x0, 0xe2, 0x9, 0xb1, 0x18, 0xc0,
    0x0, 0x9f, 0xea, 0x10,

    /* U+0044 "D" */
    0xf, 0xff, 0xd5, 0x0, 0xf, 0x10, 0x4e, 0x50,
    0xf, 0x10, 0x4, 0xd0, 0xf, 0x10, 0x0, 0xf1,
    0xf, 0x10, 0x0, 0xf2, 0xf, 0x10, 0x0, 0xf1,
    0xf, 0x10, 0x5, 0xd0, 0xf, 0x10, 0x4e, 0x50,
    0xf, 0xff, 0xd5, 0x0,

    /* U+0045 "E" */
    0xf, 0xff, 0xff, 0x50, 0xf1, 0x0, 0x0, 0xf,
    0x10, 0x0, 0x0, 0xf1, 0x0, 0x0, 0xf, 0xff,
    0xfd, 0x0, 0xf1, 0x0, 0x0, 0xf, 0x10, 0x0,
    0x0, 0xf1, 0x0, 0x0, 0xf, 0xff, 0xff, 0x60,

    /* U+0046 "F" */
    0xf, 0xff, 0xff, 0x40, 0xf1, 0x0, 0x0, 0xf,
    0x10, 0x0, 0x0, 0xf1, 0x0, 0x0, 0xf, 0xff,
    0xfa, 0x0, 0xf1, 0x0, 0x0, 0xf, 0x10, 0x0,
    0x0, 0xf1, 0x0, 0x0, 0xf, 0x10, 0x0, 0x0,

    /* U+0047 "G" */
    0x0, 0x9e, 0xfb, 0x20, 0x9, 0xc2, 0x7, 0xe0,
    0xf, 0x20, 0x0, 0x61, 0x3e, 0x0, 0x0, 0x0,
    0x4d, 0x0, 0xcf, 0xf4, 0x3e, 0x0, 0x0, 0xd4,
    0xf, 0x20, 0x0, 0xd4, 0x8, 0xd2, 0x4, 0xf3,
    0x0, 0x7e, 0xfd, 0x60,

    /* U+0048 "H" */
    0xf, 0x10, 0x0, 0x98, 0xf, 0x10, 0x0, 0x98,
    0xf, 0x10, 0x0, 0x98, 0xf, 0x10, 0x0, 0x98,
    0xf, 0xff, 0xff, 0xf8, 0xf, 0x10, 0x0, 0x98,
    0xf, 0x10, 0x0, 0x98, 0xf, 0x10, 0x0, 0x98,
    0xf, 0x10, 0x0, 0x98,

    /* U+0049 "I" */
    0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3,
    0xe3,

    /* U+004A "J" */
    0x0, 0x0, 0x7b, 0x0, 0x0, 0x7b, 0x0, 0x0,
    0x7b, 0x0, 0x0, 0x7b, 0x0, 0x0, 0x7b, 0x0,
    0x0, 0x7b, 0x64, 0x0, 0x7a, 0x7c, 0x11, 0xd6,
    0x9, 0xee, 0x90,

    /* U+004B "K" */
    0xf, 0x10, 0x8, 0xc0, 0xf, 0x10, 0x6e, 0x10,
    0xf, 0x13, 0xe3, 0x0, 0xf, 0x4e, 0x40, 0x0,
    0xf, 0xef, 0x40, 0x0, 0xf, 0x85, 0xe1, 0x0,
    0xf, 0x10, 0x9b, 0x0, 0xf, 0x10, 0xd, 0x70,
    0xf, 0x10, 0x3, 0xf3,

    /* U+004C "L" */
    0xf, 0x20, 0x0, 0x0, 0xf2, 0x0, 0x0, 0xf,
    0x20, 0x0, 0x0, 0xf2, 0x0, 0x0, 0xf, 0x20,
    0x0, 0x0, 0xf2, 0x0, 0x0, 0xf, 0x20, 0x0,
    0x0, 0xf2, 0x0, 0x0, 0xf, 0xff, 0xff, 0x20,

    /* U+004D "M" */
    0xf, 0xa0, 0x0, 0x2, 0xf7, 0xf, 0xf0, 0x0,
    0x8, 0xf7, 0xf, 0xb6, 0x0, 0xe, 0xb7, 0xf,
    0x5c, 0x0, 0x4c, 0x97, 0xf, 0x1e, 0x20, 0xb6,
    0x97, 0xf, 0x18, 0x81, 0xe0, 0xa7, 0xf, 0x12,
    0xe7, 0x90, 0xa7, 0xf, 0x10, 0xcf, 0x30, 0xa7,
    0xf, 0x10, 0x6d, 0x0, 0xa7,

    /* U+004E "N" */
    0xf, 0x60, 0x0, 0x98, 0xf, 0xf1, 0x0, 0x98,
    0xf, 0xba, 0x0, 0x98, 0xf, 0x3e, 0x40, 0x98,
    0xf, 0x25, 0xd0, 0x98, 0xf, 0x20, 0xb8, 0x98,
    0xf, 0x20, 0x2f, 0xb8, 0xf, 0x20, 0x8, 0xf8,
    0xf, 0x20, 0x0, 0xd8,

    /* U+004F "O" */
    0x0, 0x8e, 0xfa, 0x10, 0x9, 0xc3, 0x29, 0xd0,
    0x1f, 0x20, 0x0, 0xd4, 0x3e, 0x0, 0x0, 0xa7,
    0x4d, 0x0, 0x0, 0x98, 0x4e, 0x0, 0x0, 0xa7,
    0x1f, 0x20, 0x0, 0xd4, 0x9, 0xc2, 0x19, 0xd0,
    0x0, 0x8e, 0xfb, 0x10,

    /* U+0050 "P" */
    0xf, 0xff, 0xfb, 0x20, 0xf, 0x10, 0x8, 0xd0,
    0xf, 0x10, 0x0, 0xf1, 0xf, 0x10, 0x7, 0xe0,
    0xf, 0xff, 0xfc, 0x30, 0xf, 0x10, 0x0, 0x0,
    0xf, 0x10, 0x0, 0x0, 0xf, 0x10, 0x0, 0x0,
    0xf, 0x10, 0x0, 0x0,

    /* U+0051 "Q" */
    0x0, 0x9e, 0xfa, 0x10, 0xa, 0xc2, 0x2a, 0xc0,
    0x1f, 0x10, 0x0, 0xe3, 0x4d, 0x0, 0x0, 0xb6,
    0x5c, 0x0, 0x0, 0xa7, 0x4d, 0x0, 0x0, 0xb7,
    0x1f, 0x10, 0x0, 0xe3, 0xa, 0xb2, 0x19, 0xc0,
    0x0, 0x9e, 0xff, 0x50, 0x0, 0x0, 0x5, 0xf3,
    0x0, 0x0, 0x0, 0x20,

    /* U+0052 "R" */
    0xf, 0xff, 0xe9, 0x0, 0xf, 0x10, 0x1b, 0x80,
    0xf, 0x10, 0x6, 0xc0, 0xf, 0x10, 0x1b, 0x80,
    0xf, 0xff, 0xf9, 0x0, 0xf, 0x10, 0xa8, 0x0,
    0xf, 0x10, 0x2e, 0x0, 0xf, 0x10, 0xb, 0x70,
    0xf, 0x10, 0x4, 0xe0,

    /* U+0053 "S" */
    0x4, 0xcf, 0xd6, 0x1, 0xf5, 0x3, 0xe5, 0x3e,
    0x0, 0x5, 0x60, 0xd9, 0x20, 0x0, 0x1, 0x8e,
    0xc4, 0x0, 0x0, 0x5, 0xe5, 0x57, 0x0, 0x8,
    0xa3, 0xf4, 0x2, 0xc7, 0x4, 0xcf, 0xe8, 0x0,

    /* U+0054 "T" */
    0xbf, 0xff, 0xff, 0xe0, 0x0, 0xf2, 0x0, 0x0,
    0xf, 0x20, 0x0, 0x0, 0xf2, 0x0, 0x0, 0xf,
    0x20, 0x0, 0x0, 0xf2, 0x0, 0x0, 0xf, 0x20,
    0x0, 0x0, 0xf2, 0x0, 0x0, 0xf, 0x20, 0x0,

    /* U+0055 "U" */
    0x2f, 0x0, 0x2, 0xf2, 0xf0, 0x0, 0x2f, 0x2f,
    0x0, 0x2, 0xf2, 0xf0, 0x0, 0x2f, 0x2f, 0x0,
    0x2, 0xf2, 0xf0, 0x0, 0x2f, 0x1f, 0x0, 0x3,
    0xe0, 0xc9, 0x11, 0xb9, 0x2, 0xbf, 0xe9, 0x0,

    /* U+0056 "V" */
    0xa8, 0x0, 0x0, 0xe4, 0x4d, 0x0, 0x3, 0xe0,
    0xe, 0x20, 0x8, 0x90, 0x9, 0x80, 0xd, 0x40,
    0x4, 0xd0, 0x3e, 0x0, 0x0, 0xe2, 0x88, 0x0,
    0x0, 0x87, 0xd3, 0x0, 0x0, 0x3e, 0xd0, 0x0,
    0x0, 0xd, 0x70, 0x0,

    /* U+0057 "W" */
    0x89, 0x0, 0x3f, 0x0, 0xd, 0x44, 0xc0, 0x7,
    0xf4, 0x1, 0xf0, 0x1f, 0x0, 0xca, 0x80, 0x4d,
    0x0, 0xd3, 0xe, 0x3c, 0x7, 0x90, 0x9, 0x74,
    0xb0, 0xe0, 0xb5, 0x0, 0x6a, 0x86, 0xa, 0x5e,
    0x20, 0x2, 0xed, 0x20, 0x6a, 0xe0, 0x0, 0xe,
    0xd0, 0x1, 0xfa, 0x0, 0x0, 0xb9, 0x0, 0xd,
    0x70, 0x0,

    /* U+0058 "X" */
    0x4e, 0x10, 0x7, 0xd0, 0xb, 0x90, 0x1f, 0x30,
    0x2, 0xf2, 0x9a, 0x0, 0x0, 0x7d, 0xe1, 0x0,
    0x0, 0x1f, 0x90, 0x0, 0x0, 0x8d, 0xf1, 0x0,
    0x2, 0xf2, 0x9a, 0x0, 0xc, 0x90, 0x1e, 0x40,
    0x5e, 0x0, 0x6, 0xd0,

    /* U+0059 "Y" */
    0xa9, 0x0, 0x6, 0xd0, 0x2f, 0x10, 0xe, 0x40,
    0x9, 0x90, 0x6c, 0x0, 0x1, 0xe1, 0xd4, 0x0,
    0x0, 0x9c, 0xb0, 0x0, 0x0, 0x1f, 0x40, 0x0,
    0x0, 0xf, 0x20, 0x0, 0x0, 0xf, 0x20, 0x0,
    0x0, 0xf, 0x20, 0x0,

    /* U+005A "Z" */
    0x6f, 0xff, 0xff, 0x80, 0x0, 0x2, 0xf2, 0x0,
    0x0, 0xc7, 0x0, 0x0, 0x7c, 0x0, 0x0, 0x2f,
    0x20, 0x0, 0xc, 0x70, 0x0, 0x7, 0xc0, 0x0,
    0x2, 0xf2, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xb0,

    /* U+005B "[" */
    0x2f, 0xf0, 0x2f, 0x0, 0x2f, 0x0, 0x2f, 0x0,
    0x2f, 0x0, 0x2f, 0x0, 0x2f, 0x0, 0x2f, 0x0,
    0x2f, 0x0, 0x2f, 0x0, 0x2f, 0x0, 0x2f, 0x0,
    0x2f, 0xf0,

    /* U+005C "\\" */
    0x97, 0x0, 0x3, 0xc0, 0x0, 0xd, 0x20, 0x0,
    0x88, 0x0, 0x2, 0xd0, 0x0, 0xc, 0x30, 0x0,
    0x79, 0x0, 0x1, 0xe0, 0x0, 0xb, 0x40, 0x0,
    0x6a,

    /* U+005D "]" */
    0xef, 0x40, 0xd4, 0xd, 0x40, 0xd4, 0xd, 0x40,
    0xd4, 0xd, 0x40, 0xd4, 0xd, 0x40, 0xd4, 0xd,
    0x40, 0xd4, 0xef, 0x40,

    /* U+005E "^" */
    0x0, 0x70, 0x0, 0x4f, 0x40, 0xa, 0x9a, 0x1,
    0xe0, 0xe1, 0x69, 0x9, 0x60,

    /* U+005F "_" */
    0xef, 0xff, 0xf5,

    /* U+0061 "a" */
    0x4, 0xde, 0xb0, 0x1f, 0x30, 0xb7, 0x2, 0x0,
    0x7a, 0x5, 0xcd, 0xea, 0x3e, 0x10, 0x7a, 0x4e,
    0x12, 0xca, 0x9, 0xfd, 0x8b,

    /* U+0062 "b" */
    0x2e, 0x0, 0x0, 0x2, 0xe0, 0x0, 0x0, 0x2e,
    0x0, 0x0, 0x2, 0xe9, 0xec, 0x20, 0x2f, 0x50,
    0x8c, 0x2, 0xe0, 0x0, 0xf1, 0x2e, 0x0, 0xe,
    0x22, 0xe0, 0x0, 0xf1, 0x2f, 0x50, 0x8c, 0x2,
    0xd9, 0xec, 0x20,

    /* U+0063 "c" */
    0x4, 0xdf, 0xb1, 0x1f, 0x40, 0x9a, 0x5b, 0x0,
    0x16, 0x6a, 0x0, 0x0, 0x5b, 0x0, 0x3, 0x1e,
    0x40, 0x8a, 0x3, 0xdf, 0xa0,

    /* U+0064 "d" */
    0x0, 0x0, 0x2e, 0x0, 0x0, 0x2e, 0x0, 0x0,
    0x2e, 0x4, 0xde, 0x9e, 0x1f, 0x51, 0x9e, 0x5c,
    0x0, 0x2e, 0x6a, 0x0, 0x2e, 0x5b, 0x0, 0x2e,
    0xe, 0x30, 0x8e, 0x4, 0xdd, 0x9e,

    /* U+0065 "e" */
    0x3, 0xdf, 0xa0, 0xe, 0x50, 0xa8, 0x5c, 0x0,
    0x3c, 0x6f, 0xff, 0xfe, 0x5b, 0x0, 0x0, 0x1e,
    0x40, 0x47, 0x3, 0xcf, 0xc3,

    /* U+0066 "f" */
    0x1, 0xcf, 0x20, 0x8a, 0x0, 0xa, 0x70, 0x9,
    0xff, 0xb0, 0xa, 0x70, 0x0, 0xa7, 0x0, 0xa,
    0x70, 0x0, 0xa7, 0x0, 0xa, 0x70, 0x0, 0xa7,
    0x0,

    /* U+0067 "g" */
    0x4, 0xde, 0x8e, 0x1f, 0x51, 0x9e, 0x5c, 0x0,
    0x2e, 0x6a, 0x0, 0x2e, 0x5c, 0x0, 0x2e, 0xf,
    0x51, 0x9e, 0x4, 0xde, 0x9e, 0x0, 0x0, 0x4d,
    0xc, 0x20, 0xb9, 0x6, 0xde, 0xa0,

    /* U+0068 "h" */
    0x2e, 0x0, 0x0, 0x2e, 0x0, 0x0, 0x2e, 0x0,
    0x0, 0x2e, 0x8e, 0xd2, 0x2f, 0x60, 0x9a, 0x2e,
    0x0, 0x4c, 0x2e, 0x0, 0x4d, 0x2e, 0x0, 0x4d,
    0x2e, 0x0, 0x4d, 0x2e, 0x0, 0x4d,

    /* U+0069 "i" */
    0x1e, 0x0, 0x30, 0x1f, 0x1, 0xf0, 0x1f, 0x1,
    0xf0, 0x1f, 0x1, 0xf0, 0x1f, 0x0,

    /* U+006A "j" */
    0x2, 0xd0, 0x0, 0x30, 0x2, 0xf0, 0x2, 0xf0,
    0x2, 0xf0, 0x2, 0xf0, 0x2, 0xf0, 0x2, 0xf0,
    0x2, 0xf0, 0x2, 0xe0, 0x4, 0xd0, 0x5f, 0x60,

    /* U+006B "k" */
    0x2e, 0x0, 0x0, 0x2, 0xe0, 0x0, 0x0, 0x2e,
    0x0, 0x0, 0x2, 0xe0, 0x1d, 0x50, 0x2e, 0xc,
    0x80, 0x2, 0xea, 0xa0, 0x0, 0x2f, 0xfb, 0x0,
    0x2, 0xf2, 0xd6, 0x0, 0x2e, 0x3, 0xe1, 0x2,
    0xe0, 0x8, 0xb0,

    /* U+006C "l" */
    0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
    0x1f, 0x1f,

    /* U+006D "m" */
    0x3e, 0x9e, 0xd3, 0xbf, 0xc1, 0x3f, 0x50, 0x9f,
    0x40, 0xa8, 0x3e, 0x0, 0x4d, 0x0, 0x6b, 0x3e,
    0x0, 0x4c, 0x0, 0x6b, 0x3e, 0x0, 0x4c, 0x0,
    0x6b, 0x3e, 0x0, 0x4c, 0x0, 0x6b, 0x3e, 0x0,
    0x4c, 0x0, 0x6b,

    /* U+006E "n" */
    0x2d, 0x8e, 0xd2, 0x2f, 0x60, 0x9a, 0x2e, 0x0,
    0x4c, 0x2e, 0x0, 0x4d, 0x2e, 0x0, 0x4d, 0x2e,
    0x0, 0x4d, 0x2e, 0x0, 0x4d,

    /* U+006F "o" */
    0x3, 0xcf, 0xb1, 0x1, 0xe5, 0x7, 0xc0, 0x5c,
    0x0, 0xe, 0x27, 0xa0, 0x0, 0xc4, 0x5c, 0x0,
    0xe, 0x30, 0xe5, 0x7, 0xd0, 0x3, 0xcf, 0xc2,
    0x0,

    /* U+0070 "p" */
    0x2e, 0xae, 0xc2, 0x2, 0xf3, 0x7, 0xc0, 0x2e,
    0x0, 0xf, 0x2, 0xe0, 0x0, 0xe2, 0x2e, 0x0,
    0xf, 0x2, 0xf4, 0x8, 0xc0, 0x2e, 0xae, 0xc2,
    0x2, 0xe0, 0x0, 0x0, 0x2e, 0x0, 0x0, 0x2,
    0xe0, 0x0, 0x0,

    /* U+0071 "q" */
    0x4, 0xde, 0x9e, 0x1f, 0x40, 0x8e, 0x5c, 0x0,
    0x3e, 0x6a, 0x0, 0x3e, 0x5c, 0x0, 0x3e, 0x1f,
    0x40, 0x8e, 0x4, 0xde, 0xae, 0x0, 0x0, 0x3e,
    0x0, 0x0, 0x3e, 0x0, 0x0, 0x3e,

    /* U+0072 "r" */
    0x0, 0x0, 0x2e, 0xbd, 0x2f, 0x50, 0x2e, 0x0,
    0x2e, 0x0, 0x2e, 0x0, 0x2e, 0x0, 0x2e, 0x0,

    /* U+0073 "s" */
    0x6, 0xee, 0x90, 0x1f, 0x21, 0xc6, 0x1f, 0x40,
    0x0, 0x4, 0xce, 0x90, 0x12, 0x1, 0xb7, 0x4e,
    0x10, 0xa7, 0x7, 0xee, 0xa0,

    /* U+0074 "t" */
    0x6, 0x20, 0xc, 0x40, 0xdf, 0xe6, 0xc, 0x40,
    0xc, 0x40, 0xc, 0x40, 0xc, 0x40, 0xc, 0x60,
    0x5, 0xf7,

    /* U+0075 "u" */
    0x3e, 0x0, 0x4c, 0x3e, 0x0, 0x4c, 0x3e, 0x0,
    0x4c, 0x3e, 0x0, 0x4c, 0x2e, 0x0, 0x4c, 0x1f,
    0x31, 0xbc, 0x7, 0xee, 0x9c,

    /* U+0076 "v" */
    0xa7, 0x0, 0xa6, 0x5c, 0x0, 0xf1, 0xf, 0x14,
    0xc0, 0xa, 0x59, 0x70, 0x5, 0xad, 0x20, 0x0,
    0xfc, 0x0, 0x0, 0xa7, 0x0,

    /* U+0077 "w" */
    0x97, 0x1, 0xf1, 0x7, 0x95, 0xa0, 0x5f, 0x50,
    0xb5, 0x1e, 0xa, 0x8a, 0xe, 0x10, 0xd2, 0xd0,
    0xe2, 0xc0, 0x9, 0x9a, 0xb, 0x98, 0x0, 0x4f,
    0x60, 0x6f, 0x40, 0x0, 0xf1, 0x2, 0xf0, 0x0,

    /* U+0078 "x" */
    0x6d, 0x0, 0xe5, 0xc, 0x67, 0xb0, 0x3, 0xee,
    0x20, 0x0, 0xcb, 0x0, 0x4, 0xde, 0x20, 0xd,
    0x56, 0xc0, 0x7c, 0x0, 0xd6,

    /* U+0079 "y" */
    0xb7, 0x0, 0xc6, 0x6c, 0x1, 0xf1, 0x1f, 0x15,
    0xc0, 0xc, 0x6a, 0x70, 0x7, 0xbe, 0x20, 0x1,
    0xfd, 0x0, 0x0, 0xc8, 0x0, 0x0, 0xb3, 0x0,
    0x4, 0xd0, 0x0, 0x7e, 0x40, 0x0,

    /* U+007A "z" */
    0x6f, 0xff, 0xf5, 0x0, 0x5, 0xd0, 0x0, 0x1e,
    0x40, 0x0, 0xb9, 0x0, 0x6, 0xd0, 0x0, 0x1e,
    0x30, 0x0, 0x7f, 0xff, 0xf8,

    /* U+007B "{" */
    0x0, 0x27, 0x0, 0xe4, 0x4, 0xc0, 0x5, 0xb0,
    0x6, 0xa0, 0xa, 0x70, 0x9e, 0x0, 0xa, 0x70,
    0x6, 0xa0, 0x5, 0xb0, 0x4, 0xc0, 0x0, 0xe4,
    0x0, 0x27,

    /* U+007C "|" */
    0xee, 0xee, 0xee, 0xee, 0xee, 0xe0,

    /* U+007D "}" */
    0x72, 0x0, 0x3e, 0x0, 0xb, 0x50, 0xa, 0x60,
    0xa, 0x60, 0x7, 0xb0, 0x0, 0xea, 0x7, 0xb0,
    0xa, 0x60, 0xa, 0x60, 0xb, 0x50, 0x4e, 0x0,
    0x72, 0x0,

    /* U+007E "~" */
    0x5, 0xd9, 0x0, 0x75, 0xe, 0x3a, 0x90, 0xc2,
    0x2a, 0x0, 0xbf, 0x90, 0x0, 0x0, 0x0, 0x0,

    /* U+00B0 "°" */
    0x9, 0xc3, 0x2a, 0x2a, 0xa, 0xd4,

    /* U+02DA "˚" */
    0x9, 0x80, 0x46, 0x73, 0x9, 0x80,

    /* U+2014 "—" */
    0x2f, 0xff, 0xff, 0xff, 0xa0,

    /* U+201C "“" */
    0x0, 0x0, 0x0, 0xb2, 0xc0, 0x4c, 0x6a, 0x6,
    0xb8, 0x80,

    /* U+201D "”" */
    0x1f, 0x4c, 0x2d, 0x6a, 0x76, 0xa3, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 49, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 14, .adv_w = 61, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 22, .adv_w = 118, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 108, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 100, .adv_w = 141, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 119, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 33, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 181, .adv_w = 66, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 209, .adv_w = 67, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 237, .adv_w = 83, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 252, .adv_w = 109, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 277, .adv_w = 38, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 281, .adv_w = 53, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 283, .adv_w = 51, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 286, .adv_w = 79, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 311, .adv_w = 108, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 343, .adv_w = 108, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 361, .adv_w = 108, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 108, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 420, .adv_w = 108, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 108, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 484, .adv_w = 108, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 516, .adv_w = 108, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 548, .adv_w = 108, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 580, .adv_w = 108, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 607, .adv_w = 47, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 618, .adv_w = 41, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 627, .adv_w = 98, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 645, .adv_w = 105, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 660, .adv_w = 100, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 678, .adv_w = 91, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 705, .adv_w = 172, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 771, .adv_w = 125, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 807, .adv_w = 120, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 839, .adv_w = 125, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 875, .adv_w = 126, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 911, .adv_w = 109, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 943, .adv_w = 106, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 975, .adv_w = 131, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1011, .adv_w = 137, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1047, .adv_w = 52, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1056, .adv_w = 106, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1083, .adv_w = 120, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1119, .adv_w = 103, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1151, .adv_w = 168, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1196, .adv_w = 137, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1232, .adv_w = 132, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1268, .adv_w = 121, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1304, .adv_w = 132, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1348, .adv_w = 118, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1384, .adv_w = 114, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1416, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1448, .adv_w = 125, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1480, .adv_w = 122, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1516, .adv_w = 170, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1566, .adv_w = 120, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1602, .adv_w = 115, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1638, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1670, .adv_w = 51, .box_w = 4, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1696, .adv_w = 79, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1721, .adv_w = 51, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1741, .adv_w = 80, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 1754, .adv_w = 87, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1757, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1778, .adv_w = 108, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1813, .adv_w = 101, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1834, .adv_w = 108, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1864, .adv_w = 102, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1885, .adv_w = 67, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1910, .adv_w = 108, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1940, .adv_w = 106, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1970, .adv_w = 47, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1984, .adv_w = 46, .box_w = 4, .box_h = 12, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 2008, .adv_w = 97, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2043, .adv_w = 47, .box_w = 2, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2053, .adv_w = 168, .box_w = 10, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2088, .adv_w = 106, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2109, .adv_w = 110, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2134, .adv_w = 108, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 2169, .adv_w = 109, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 2199, .adv_w = 65, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2215, .adv_w = 99, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2236, .adv_w = 63, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2254, .adv_w = 106, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2275, .adv_w = 93, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2296, .adv_w = 144, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2328, .adv_w = 95, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2349, .adv_w = 91, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 2379, .adv_w = 95, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2400, .adv_w = 65, .box_w = 4, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 2426, .adv_w = 47, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 2432, .adv_w = 65, .box_w = 4, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 2458, .adv_w = 131, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 2474, .adv_w = 72, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 2480, .adv_w = 64, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 2486, .adv_w = 150, .box_w = 9, .box_h = 1, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 2491, .adv_w = 68, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 2501, .adv_w = 69, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 6}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_2[] = {
    0x0, 0x22a, 0x1f64, 0x1f6c, 0x1f6d
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 33, .range_length = 63, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 30, .glyph_id_start = 64,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 176, .range_length = 8046, .glyph_id_start = 94,
        .unicode_list = unicode_list_2, .glyph_id_ofs_list = NULL, .list_length = 5, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 1, 0, 0, 0, 0, 1,
    2, 0, 0, 0, 3, 0, 3, 4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 5, 6, 7, 8, 9, 10, 0,
    11, 11, 12, 13, 14, 11, 11, 8,
    15, 16, 17, 0, 18, 12, 19, 20,
    21, 22, 23, 24, 0, 0, 0, 0,
    25, 26, 27, 0, 28, 29, 0, 30,
    0, 0, 31, 0, 30, 30, 32, 26,
    0, 33, 0, 34, 0, 35, 36, 37,
    35, 38, 39, 0, 0, 0, 0, 0,
    0, 1, 1
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 1, 0, 0, 0, 2, 1,
    0, 3, 4, 0, 5, 6, 5, 7,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 8,
    0, 9, 0, 10, 0, 0, 0, 10,
    0, 0, 11, 0, 0, 0, 0, 10,
    0, 10, 0, 12, 13, 14, 15, 16,
    17, 18, 19, 0, 0, 20, 0, 0,
    21, 0, 22, 22, 22, 23, 22, 0,
    0, 0, 0, 0, 24, 24, 25, 24,
    22, 26, 27, 28, 29, 30, 31, 32,
    30, 33, 0, 0, 34, 0, 0, 0,
    6, 1, 1
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    -10, 0, 0, 0, 0, 0, 0, 0,
    -11, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -5, -6, 0, -2,
    -6, 0, -7, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 0, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -16, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -21, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -11, 0, 0, 0, 0, 0, 0, -6,
    0, -1, 0, 0, -12, -2, -8, -6,
    0, -9, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, -2, -1, -5, -3, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    -2, 0, 0, -5, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -3, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    0, 0, -10, 0, 0, 0, -2, 0,
    0, 0, -3, 0, -2, 0, -2, -4,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, 0, 0,
    0, 0, 0, 0, 0, -2, -2, 0,
    -2, 0, 0, 0, -2, -2, -2, 0,
    0, 0, 0, 0, 0, 0, -22, 0,
    0, 0, -16, 0, -25, 0, 2, 0,
    0, 0, 0, 0, 0, 0, -3, -2,
    0, 0, -2, -2, 0, 0, -2, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, 0, 0,
    -3, 0, 0, 0, 2, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, 0,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, -2,
    -3, 0, 0, 0, -2, -4, -6, 0,
    0, 0, -31, 0, 0, 0, 0, 0,
    0, 0, 2, -6, 0, 0, -26, -5,
    -16, -13, 0, -22, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, -12,
    -9, 0, 0, 0, 0, 0, 0, 0,
    -30, 0, 0, 0, -13, 0, -19, 0,
    0, 0, 0, 0, -3, 0, -2, 0,
    -1, -1, 0, 0, -1, 0, 0, 1,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -4, 0, -3, -2, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -7, 0, -2, 0,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -20, -22,
    0, 0, -7, -3, -22, -1, 2, 0,
    2, 1, 0, 2, 0, 0, -11, -9,
    0, -10, -9, -7, -11, 0, -9, -7,
    -5, -7, -6, 0, 0, 0, 2, 0,
    -21, -3, 0, 0, -7, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2,
    -4, -4, 0, 0, -4, -3, 0, 0,
    -3, -1, 0, 0, 0, 2, 0, 0,
    1, 0, -12, -6, 0, 0, -4, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 1, -3, -3, 0, 0, -3, -2,
    0, 0, -2, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, -4, 0, 0,
    0, -2, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    -2, 0, 0, 0, -2, -3, 0, 0,
    0, 0, 0, -3, 2, -5, -20, -5,
    0, 0, -9, -3, -9, -1, 2, -9,
    2, 2, 1, 2, 0, 2, -7, -6,
    -2, -4, -6, -4, -5, -2, -4, -2,
    0, -2, -3, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 1, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, -2, 0, 0, 0,
    -2, -3, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, -2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0,
    0, 0, -3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, -1, -1, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    2, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, -10, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -13, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, -2, -1, 0,
    2, 0, 0, 0, -12, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -4, -2, 1, 0,
    -2, 0, 0, 5, 0, 2, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0,
    -10, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -1, 1, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -12, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 39,
    .right_class_cnt     = 34,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 3,
    .bpp = 4,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t lv_font_123abc_12 = {
#else
lv_font_t lv_font_123abc_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 14,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if LV_FONT_123ABC_12*/
