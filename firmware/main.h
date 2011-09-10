#ifndef _MAIN_H
#define _MAIN_H

#include <inttypes.h>

#define LED_WIDTH   4
#define LED_HEIGHT  5

#define ATTRIBUTES	__attribute__ ((naked, used, section (".init8")));

void setLed(uint8_t,uint8_t); 			// led_nr , brightness
void setLedXY(uint8_t,uint8_t,uint8_t); // x , y , brightness
void scrollLeft(void);
void registerAnimation(uint8_t (*)(void),uint16_t,uint16_t);

#endif
