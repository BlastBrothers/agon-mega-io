#ifndef _SCREEN_CONFIG_H
#define _SCREEN_CONFIG_H

#include <agon/vdp_vdu.h>

/**
    Sets current screen mode.
    AgDev defines "vdp_mode" but that's currently limited to modes 0-3
*/
void set_screen_mode(uint8_t mode);

/**
    Enables/disables pre-VDP 1.04 screen modes.
*/
void legacy_screen_modes(bool flag);

/**
    Sets a color in the current palette to a color from the Agon master palette.

    @param index Target palette index (0-63 but depends on screen mode)
    @param color Which color to set it to (0-63, maps to 0b00RRGGBB)
*/
void set_pal_color(uint8_t index, uint8_t color);

/**
    Sets a color in the current palette to an RGB222 color.

    @param index Target palette index (0-63)
    @param r Red (0-255, maps to 0bRR000000)
    @param g Green (0-255, maps to 0bGG000000)
    @param b Blue (0-255, maps to 0bBB000000)
*/
void set_pal_color_by_channel(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
#endif