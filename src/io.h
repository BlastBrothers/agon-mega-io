#ifndef _IO_H
#define _IO_H

#include <agon/vdp_vdu.h>

/**
    Sends a 16-but value to the VDP.
    Code is taken from HeathenUK's agon_mod repository.
*/
void put16(uint16_t w);

/**
    Sends a 24-bit value to the VDP.
    Code is taken from HeathenUK's agon_mod repository.
*/
void put24(uint24_t w);

unsigned char get_port(uint8_t port);
void set_port(uint8_t port, uint8_t value);

#endif