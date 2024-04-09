#include <mos_api.h>
#include <agon/vdp_vdu.h>
#include "io.h"

void set_screen_mode(uint8_t mode) {
    putch(22);
    putch(mode);
}

void legacy_screen_modes(bool flag) {
    const uint8_t cmd[] = { 23, 0, 0xC1, flag };
    VDP_PUTS(cmd);
}

void set_pal_color(uint8_t index, uint8_t color) {
    const uint8_t cmd[] = { 19, index, color, 0, 0, 0 };
    VDP_PUTS(cmd);
}

void set_pal_color_by_channel(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    const uint8_t cmd[] = { 19, index, 255, r, g, b };
    VDP_PUTS(cmd);
}