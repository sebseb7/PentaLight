#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define LED_WIDTH	4
#define LED_HEIGHT	5

#define SIMULATOR

#define ATTRIBUTES	__attribute__((constructor));

typedef enum {
	KEY_A,
	KEY_B,
} key_type;

typedef enum {
	DOWN,
	UP,
} event_type;

typedef uint8_t (*tick_fun)(void);
typedef void (*key_fun)(key_type key, event_type event);

void setLedXY(uint8_t x, uint8_t y, uint8_t brightness);
void registerAnimation(tick_fun tick, uint16_t t, uint16_t duration);
void registerApp(tick_fun tick, key_fun key, uint16_t t);

#endif
