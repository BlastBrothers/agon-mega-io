#include <mos_api.h>
#include <agon/vdp_key.h>
#include "io.h"
#include "screen_config.h"
#include <agon/vdp_vdu.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controller.h"
#include "button_display.h"

static volatile SYSVAR* sv; //pointer to MOS sysvars

void game_loop();
void key_event_handler(KEY_EVENT key_event);

void binary_print(char const* s, uint8_t n);
void binary_print(char const* s, uint24_t n);
void binary_print(char const* s, uint16_t n);
void binary_print(char const* s, int8_t n);
void binary_print(char const* s, int16_t n);
void print_ports(uint8_t portB, uint8_t portC, uint8_t portD, uint8_t x, uint8_t y);

int main() {
    //program start boilerplate
    sv = vdp_vdu_init();
    if (vdp_key_init() == -1)
        return 1;
    vdp_set_key_event_handler(key_event_handler);
    legacy_screen_modes(false);
    set_screen_mode(1); //640x480, 4 colors
    vdp_cursor_enable(false);
    vdp_set_text_colour(3);
    vdp_logical_scr_dims(false);

    //set up a neat looking color palette
    set_pal_color(0, 0b00000000); //black
    set_pal_color(1, 0b00010000); //dark red
    set_pal_color(2, 0b00001000); //green
    set_pal_color(3, 0b00111110); //off-white

    //enable mouse cursor
    putch(23);
    putch(0);
    putch(0x89);
    putch(0);

    //select a bitmap to use for the mouse sprite
    putch(23);
    putch(0);
    putch(0x89);
    putch(3);
    put16(1); //mouse style

    init_detect_controllers();

    init_button_display();

    //main loop
    uint24_t buttons = 0xFFFFFF;
    uint24_t prev_buttons = 0;
    uint8_t prev_mouseButtons = 0;
    int8_t prev_mouseWheel = 0;
    uint8_t mouseButtons = 0;
    int8_t mouseWheel = 0;
    while (true) {
        uint24_t time_start = sv->time;
        vdp_update_key_state(); //keyboard handler

        prev_buttons = buttons;
        buttons = read_controllers();
        update_controller_display(buttons, prev_buttons);


        prev_mouseButtons = mouseButtons;
        prev_mouseWheel = mouseWheel;
        mouseButtons = sv->mouseButtons;
        mouseWheel = sv->mouseWheel;

        update_mouse_display(mouseButtons, mouseWheel, prev_mouseButtons, prev_mouseWheel);

        vdp_cursor_tab(0, 0);
        binary_print("Buttons ", buttons);
        vdp_cursor_tab(1, 0);
        binary_print("Mouse wheel ", mouseWheel);
        vdp_cursor_tab(1, 21);
        binary_print("Buttons ", mouseButtons);

        //Lag check: sv-> time updates on a per-frame basis
        uint24_t time_end = sv->time;

        vdp_cursor_tab(59, 0);
        if (time_start != time_end) {
            printf("LAG");
        } else {
            printf("   ");
        }

        waitvblank();
    }

    return 0;
}


void print_ports(uint8_t portB, uint8_t portC, uint8_t portD, uint8_t x, uint8_t y) {
    vdp_cursor_tab(y, x);
    binary_print("portB: ", portB);
    vdp_cursor_tab(y + 1, x);
    binary_print("portC: ", portC);
    vdp_cursor_tab(y + 2, x);
    binary_print("portD: ", portD);
}

void binary_print(char const* s, uint8_t n) {
    printf("%s", s);
    for (uint8_t i = 1 + (UINT8_MAX / 2); i > 0; i = i / 2) {
        printf("%d", (n & i) > 0);
    }
}

void binary_print(char const* s, int8_t n) {
    printf("%s", s);
    for (uint8_t i = 1 + (UINT8_MAX / 2); i > 0; i = i / 2) {
        printf("%d", (n & i) > 0);
    }
}

void binary_print(char const* s, int16_t n) {
    printf("%s", s);
    for (uint16_t i = 1 + (UINT16_MAX / 2); i > 0; i = i / 2) {
        printf("%d", (n & i) > 0);
    }
}

void binary_print(char const* s, uint16_t n) {
    printf("%s", s);
    for (uint16_t i = 1 + (UINT16_MAX / 2); i > 0; i = i / 2) {
        printf("%d", (n & i) > 0);
    }
}

void binary_print(char const* s, uint24_t n) {
    //TODO for some reason doing it this way absolutely TANKS emulator performance even though it's fine on real HW
    char bin_str[25];

    uint24_t j = 0;
    for (uint24_t i = 1 + (UINT24_MAX / 2); i > 0; i = i / 2) {
        bin_str[j] = (n & i) ? '1' : '0';
        j++;
    }
    bin_str[24] = '\0';
    printf("%s%s", s, bin_str);

    //This way is slower on HW but faster on emulator
    // printf("%s", s);
    // for (uint24_t i = 1 + (UINT24_MAX / 2); i > 0; i = i / 2) {
    //     printf("%d", (n & i) > 0);
    // }
}


//Just checks if we've pressed escape and exits the program if so
void key_event_handler(KEY_EVENT key_event) {
    if (key_event.down) {
        switch (key_event.code) {
            case 0x7d: // Esc
                vdp_cursor_enable(true);
                printf("\n");
                exit(1);
                break;
        }
    }
}