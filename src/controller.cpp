#include "controller.h"
#include "io.h"
#include <stdio.h>

uint8_t pad1_type;
uint8_t pad2_type;

namespace {
    void set_select(bool i) {
        uint8_t portB_data = get_port(PORTB_DATA);
        //6 button controllers are especially sensitive to timing, expecting SELECT to be down for ~4.6 us and up for ~12.6.
        //The values of j are magic numbers designed to result in these timings.
        int24_t j = 0;
        if (i == false) {
            j = 9;
            set_port(PORTB_DATA, (portB_data & 0b11011111)); //set pin 6 low
        } else {
            j = 34;
            set_port(PORTB_DATA, (portB_data | 0b00100000)); //set pin 6 high

        }
        //Timewaste loop I stole from StackOverflow
        //If we don't "use" the variable we iterate over, and mark that as volatile, then it gets optimized away.
        volatile int sink;
        do {
            sink = 0;
        } while (--j > 0);
        (void)sink;
    }
}

//Autodetects controllers, if possible. If not, it will print instructions to the screen.
void init_detect_controllers() {

    pad1_type = CONTROLLER_UNKNOWN;
    pad2_type = CONTROLLER_UNKNOWN;


    //Setting up pin 6 of Port B to act as SELECT for both controllers.
    uint8_t portB_dir = get_port(PORTB_DIR);
    portB_dir = (portB_dir & 0b11011111); //set bit 6 of port B to 0 (output) and don't touch the rest
    set_port(PORTB_DIR, portB_dir);


    //controller 1 detection
    //TODO this is really similar to controller 2 logic. Combine?
    while (pad1_type == CONTROLLER_UNKNOWN) {
        set_select(true);
        //mask out the Controller 2 bits and hide the unrelated bits from Port D
        uint8_t portD_run1 = get_port(PORTD_DATA) >> 4 & 0b10101010;
        set_select(false);
        uint8_t portC_run2 = get_port(PORTC_DATA) & 0b10101010;
        set_select(true);
        set_select(false);
        set_select(true);
        set_select(false);
        uint8_t portC_run6 = get_port(PORTC_DATA) & 0b10101010;
        set_select(true);

        if (portC_run6 == 0) { //6-button controllers return all directions as 0 on the 6th read
            pad1_type = CONTROLLER_6_BUTTON;
        } else if ((portC_run2 & 0b11110000) == 0) { //Genesis controllers return L/R as 0 on the 2nd read
            pad1_type = CONTROLLER_3_BUTTON;
        } else {
            //unfortunately there is no way to autodetect if a controller is 2-button, or just not connected
            vdp_cursor_tab(2, 4);
            printf("Press a button on Controller 1.");

            //Did we press a button on pad 1? No need to check against different runs if we expect a 2-button
            if ((portD_run1) != 0b00001010) {
                pad1_type = CONTROLLER_2_BUTTON;
            }
        }
        waitvblank();
    }

    vdp_cursor_tab(3, 5);
    printf("%s detected on port 1 (type %d).", pad_names[pad1_type], pad1_type);

    for (uint8_t i = 60; i > 0; i--) {
        waitvblank();
    }

    //Controller 2 detection
    while (pad2_type == 255) {
        set_select(true);
        uint8_t portD_run1 = get_port(PORTD_DATA) >> 4; //hide the unrelated bits from Port D
        set_select(false);
        uint8_t portC_run2 = get_port(PORTC_DATA);
        uint8_t portD_run2 = get_port(PORTD_DATA) >> 4;
        set_select(true);
        set_select(false);
        set_select(true);
        set_select(false);
        uint8_t portC_run6 = get_port(PORTC_DATA);
        set_select(true);

        if ((portC_run6 & 0b01010101) == 0) { //6-button controllers return all directions as 0 on the 6th read
            pad2_type = CONTROLLER_6_BUTTON;
        } else if ((portC_run2 & 0b01010000) == 0) { //Genesis controllers return L/R as 0 on the 2nd read
            pad2_type = CONTROLLER_3_BUTTON;
        } else {
            //unfortunately there is no way to autodetect if a controller is 2-button, or just not connected
            vdp_cursor_tab(2, 7);
            printf("Press a button on Controller 2 if present, or Controller 1 if not.");

            //Did we get a press on pad 1?
            if (((portD_run1 | (portD_run2 << 4)) & 0b10101010) != 0b10101010) {
                pad2_type = CONTROLLER_DISCONNECTED;
            } else if ((portD_run1 & 0b01010101) != 0b00000101) { //Did we get a press on pad 2?
                pad2_type = CONTROLLER_2_BUTTON;
            }
            waitvblank();
        }
    }

    vdp_cursor_tab(3, 8);
    printf("%s detected on port 2 (type %d).", pad_names[pad2_type], pad2_type);


}


//Returns the controls 
uint24_t read_controllers() {

    set_select(true);
    uint8_t portC_run1 = get_port(PORTC_DATA);
    uint8_t portD_run1 = get_port(PORTD_DATA);
    set_select(false);
    uint8_t portD_run2 = get_port(PORTD_DATA);
    set_select(true);
    set_select(false);
    set_select(true);
    set_select(false);
    set_select(true);
    uint8_t portC_run7 = get_port(PORTC_DATA);

    uint24_t buttons = (portC_run1 << 16) + ((portD_run1 & 0b11110000) << 8) + ((portD_run2 & 0b11110000) << 4) + portC_run7;

    //Mask out button presses that should be impossible when factoring in the controller type
    switch (pad1_type) {
        case CONTROLLER_2_BUTTON:
            buttons |= 0b000000000000101010101010; //ones represent presses we're masking out
            break;
        case CONTROLLER_3_BUTTON:
            buttons |= 0b000000000000000010101010;
            break;
    }

    switch (pad2_type) {
        case CONTROLLER_DISCONNECTED:
            buttons |= 0b010101010101010101010101; //not sure when you would read a press on a disconnected controller...
            break;
        case CONTROLLER_2_BUTTON:
            buttons |= 0b000000000000010101010101;
            break;
        case CONTROLLER_3_BUTTON:
            buttons |= 0b000000000000000001010101;
            break;
    }

    return buttons;
}
