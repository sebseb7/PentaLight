

#ifndef FONT_H
#define FONT_H


#ifdef __AVR__
#include <avr/pgmspace.h>
extern prog_uint8_t font[95][3];
#else
extern uint8_t font[95][3];
#endif

#endif //FONT_H
