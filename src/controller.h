#include <mos_api.h>

#ifndef _CONTROLLER_H
#define _CONTROLLER_H

static const uint8_t PORTB_DATA = 0x9A;
static const uint8_t PORTB_DIR = 0x9B;
static const uint8_t PORTC_DATA = 0x9E;
//static const uint8_t PORTC_DIR = 0x9F;
static const uint8_t PORTD_DATA = 0xA2;
//static const uint8_t PORTD_DIR = 0xA3;

static const uint8_t CONTROLLER_UNKNOWN = 0xFF;
static const uint8_t CONTROLLER_DISCONNECTED = 0x00;
static const uint8_t CONTROLLER_2_BUTTON = 0x01;
static const uint8_t CONTROLLER_3_BUTTON = 0x02;
static const uint8_t CONTROLLER_6_BUTTON = 0x03;

static const char* pad_names[] = { "No controller", "2-button pad", "3-button pad", "6-button pad" };

extern uint8_t pad1_type;
extern uint8_t pad2_type;

uint24_t read_controllers();
void init_detect_controllers();

#endif