#include "button_display.h"
#include "controller.h"
#include <stdio.h>
#include <stdlib.h>

//draw initial state of button/mouse display based on pad types, not showing buttons that don't exist.
//This uses a bunch of magic array indices but I don't care.
void init_button_display() {
    switch (pad1_type) {
        case CONTROLLER_6_BUTTON:
            button_visuals[22].display();
            button_visuals[20].display();
            button_visuals[18].display();
            button_visuals[16].display();
        case CONTROLLER_3_BUTTON:
            button_visuals[14].display();
            button_visuals[12].display();
        case CONTROLLER_2_BUTTON:
            button_visuals[10].display();
            button_visuals[8].display();
            button_visuals[6].display();
            button_visuals[4].display();
            button_visuals[2].display();
            button_visuals[0].display();
    }

    switch (pad2_type) {
        case CONTROLLER_6_BUTTON:
            button_visuals[23].display();
            button_visuals[21].display();
            button_visuals[19].display();
            button_visuals[17].display();
        case CONTROLLER_3_BUTTON:
            button_visuals[15].display();
            button_visuals[13].display();
        case CONTROLLER_2_BUTTON:
            button_visuals[11].display();
            button_visuals[9].display();
            button_visuals[7].display();
            button_visuals[5].display();
            button_visuals[3].display();
            button_visuals[1].display();
    }

    //TODO check if mouse is connected before doing this - not possible currently because of bugged firmware
    button_visuals[24].display();
    button_visuals[25].display();
    button_visuals[26].display();
    button_visuals[27].display();
    button_visuals[28].display();
}

void update_controller_display(uint24_t buttons, uint24_t prev_buttons) {
    //uint24_t new_button_presses = ~(prev_buttons & ~(buttons));
    uint24_t new_button_releases = prev_buttons | (~buttons);


    //update button display by iterating over each bit
    int8_t i = 23; //TODO have this be lower if neither controller is a 6-button. Ditto for 3-button.
    uint24_t mask = 1;
    while (i >= 0) {
        if (!(buttons & mask)) { //remember, 1 (true) is unpressed and 0 (false) is pressed!
            button_visuals[i].redraw_color(3);
        } else if (!(new_button_releases & mask)) {
            button_visuals[i].redraw_color(2);
        }
        mask <<= 1;
        i--;
    }
}

void update_mouse_display(uint8_t mouseButtons, int8_t mouseWheel, uint8_t prev_mouseButtons, int8_t prev_mouseWheel) {
    uint8_t new_mouseButtons_releases = prev_mouseButtons & (~mouseButtons);

    if (new_mouseButtons_releases & 0b00000001) { //left click
        button_visuals[24].redraw_color(2);
    } else if (mouseButtons & 0b00000001) {
        button_visuals[24].redraw_color(3);
    }

    if (new_mouseButtons_releases & 0b00000010) { //right click
        button_visuals[25].redraw_color(2);
    } else if (mouseButtons & 0b00000010) {
        button_visuals[25].redraw_color(3);
    }

    if (new_mouseButtons_releases & 0b00000100) { //middle click
        button_visuals[26].redraw_color(2);
    } else if (mouseButtons & 0b00000100) {
        button_visuals[26].redraw_color(3);
    }

    if (mouseWheel < 0) { //wheel up
        if (prev_mouseWheel == mouseWheel && prev_mouseWheel != 0) {
            button_visuals[27].redraw_color(2);
        } else {
            button_visuals[27].redraw_color(3);
        }
    } else if (mouseWheel > 0) { //wheel down
        if (prev_mouseWheel == mouseWheel && prev_mouseWheel != 0) {
            button_visuals[28].redraw_color(2);
        } else {
            button_visuals[28].redraw_color(3);
        }
    }
}