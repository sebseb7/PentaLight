#ifndef FONT_H
#define FONT_H

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
typedef unsigned char prog_uint8_t;
#endif

extern prog_uint8_t font[][3];

#endif //FONT_H
