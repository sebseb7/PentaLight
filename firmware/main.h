#ifndef _MAIN_H
#define _MAIN_H

#include <inttypes.h>

#define LED_WIDTH   7
#define LED_HEIGHT  5

#define ATTRIBUTES	__attribute__ ((naked, used, section (".init8")));

uint16_t volatile current_adc_value;

typedef enum {
	KEY_A,
	KEY_B,
} key_type;

typedef enum {
	DOWN,
	UP,
} event_type;


void setLed(uint8_t,uint8_t); 			// led_nr , brightness
void clearAllLeds(void);
void setLedXY(uint8_t,uint8_t,uint8_t); // x , y , brightness
void scrollLeft(void);
void registerAnimation(uint8_t (*)(void),uint16_t,uint16_t);
void registerApp(uint8_t (*)(void),void (*)(key_type,event_type),uint16_t);

#endif
