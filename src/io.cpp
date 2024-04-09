#include <mos_api.h>
#include <stdio.h>


void put16(uint16_t w) {
    putch(w & 0x00FF); // write LSB
    putch(w >> 8);   // write MSB
}

void put24(uint24_t w) {
    putch(w & 0x00FF); // write LSB
    putch(w >> 8);   // write middle
    putch(w >> 16);  // write MSB
}

/**
    Gets an eZ80 port.
    From a Discord post by HeathenUK.
*/
unsigned char get_port(uint8_t port) {
    unsigned char output;
    __asm__ volatile (
        "ld b, 0 \n"
        "ld c, %1 \n"
        "in a, (c) \n"
        "ld %0, a"
        : "=d"(output)
        : "d"(port)
        : "cc", "memory", "b", "c", "a"
        );
    return output;
}

/**
    Sets an eZ80 port.
    From a Discord post by HeathenUK.
*/
void set_port(uint8_t port, uint8_t value) {
    __asm__ volatile (
        "ld b, 0 \n"
        "ld a, %1 \n"
        "ld c, %0 \n"
        "out (c), a"
        :
    : "r"(port), "r"(value)
        : "cc", "memory", "b", "c", "a"
        );
}