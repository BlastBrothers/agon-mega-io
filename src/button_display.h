#ifndef _BUTTON_DISPLAY_H
#define _BUTTON_DISPLAY_H

#include <agon/vdp_vdu.h>
#include "io.h"
#include <string.h>

typedef struct BUTTON_VISUAL {
    uint16_t x;
    uint16_t y;
    uint16_t r;
    const char* label;
    BUTTON_VISUAL(uint16_t xpos, uint16_t ypos, uint16_t radius, const char* bLabel) : x(xpos), y(ypos), r(radius), label(bLabel) {};
    void display() {
        //set graphics color
        putch(18);
        putch(0);
        putch(1);

        //move grpahics cursor
        putch(25);
        putch(4); //Move absolute - 0x04 is a line command, but since it's a "move" and not "draw" it doesn't matter
        put16(x);
        put16(y);

        //plot circle
        putch(25);
        putch(0x99); //Circle with a point on its radius specified relative to graphics cursor
        put16(r); //x
        put16(0); //y

        //move cursor to below circle
        putch(25);
        putch(0); //relative move - note that this is relative to the circle radius position
        put16(-r - 20);
        put16(-r - 10);

        //set graphics color
        putch(18);
        putch(0);
        putch(3);

        //write text at graphics cursor - have to do this ourselves since we're not using the text cursor
        putch(5);
        for (uint8_t i = 0; i < strlen(label); i++) {
            putch(label[i]);
        }

        //set VDP back to absolute positioning so subsequent draw operations don't explode
        putch(4);
        VDP_PUTS("");
    };
    void redraw_color(uint8_t color) {

        //set graphics color
        putch(18);
        putch(0);
        putch(color);

        //move graphics cursor
        putch(25);
        putch(4); //Move absolute - 0x04 is a line command, but since it's a "move" and not "draw" it doesn't matter
        put16(x);
        put16(y);

        //plot circle
        putch(25);
        putch(0x99); //Circle with a point on its radius specified relative to graphics cursor
        put16(r); //x
        put16(0); //y

        //set VDP back to absolute positioning so subsequent draw operations don't explode
        putch(4);
        VDP_PUTS("");

        //set graphics color back to text color
        putch(18);
        putch(0);
        putch(3);
    }
} BUTTON_VISUAL;

static BUTTON_VISUAL button_visuals[] = {
    BUTTON_VISUAL(188, 168, 28, "  R"), //P1
    BUTTON_VISUAL(188, 168 + 200, 28, "  R"), //P2
    BUTTON_VISUAL(100, 168, 28, "  L"), //P1
    BUTTON_VISUAL(100, 168 + 200, 28, "  L"), //P2
    BUTTON_VISUAL(144, 212, 28, "  D"), //P1
    BUTTON_VISUAL(144, 212 + 200, 28, "  D"), //P2
    BUTTON_VISUAL(144, 124, 28, "  U"), //P1
    BUTTON_VISUAL(144, 124 + 200, 28, "  U"), //P2
    BUTTON_VISUAL(552, 196, 28, " C/2"), //P1
    BUTTON_VISUAL(552, 196 + 200, 28, " C/2"), //P2
    BUTTON_VISUAL(484, 200, 28, " B/1"), //P1
    BUTTON_VISUAL(484, 200 + 200, 28, " B/1"), //P2
    BUTTON_VISUAL(304, 200, 24, "Start"), //P1
    BUTTON_VISUAL(304, 200 + 200, 24, "Start"), //P2
    BUTTON_VISUAL(416, 212, 28, "  A"), //P1
    BUTTON_VISUAL(416, 212 + 200, 28, "  A"), //P2
    BUTTON_VISUAL(304, 136, 20, "Mode"), //P1
    BUTTON_VISUAL(304, 136 + 200, 20, "Mode"), //P2
    BUTTON_VISUAL(408, 144, 24, "  X"), //P1
    BUTTON_VISUAL(408, 144 + 200, 24, "  X"), //P2
    BUTTON_VISUAL(468, 132, 24, "  Y"), //P1
    BUTTON_VISUAL(468, 132 + 200, 24, "  Y"), //P2
    BUTTON_VISUAL(528, 128, 24, "  Z"), //P1
    BUTTON_VISUAL(528, 128 + 200, 24, "  Z"), //P2
    BUTTON_VISUAL(564, 48, 12, "  M1"),
    BUTTON_VISUAL(612, 48, 12, "  M2"),
    BUTTON_VISUAL(588, 48, 8, ""), //M3
    BUTTON_VISUAL(588, 34, 6, "  M3  "), //M3 up
    BUTTON_VISUAL(588, 62, 6, ""), //M3 down
};

void init_button_display();
void update_controller_display(uint24_t buttons, uint24_t prev_buttons);
void update_mouse_display(uint8_t mouseButtons, int8_t mouseWheel, uint8_t prev_mouseButtons, int8_t prev_mouseWheel);

#endif