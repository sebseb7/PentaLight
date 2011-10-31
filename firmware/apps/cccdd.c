#include <main.h>
#include "binary.h"

#define b00000 0
#define b00001 1
#define b00010 2
#define b00011 3
#define b00100 4
#define b00101 5
#define b00110 6
#define b00111 7
#define b01000 8
#define b01001 9
#define b01010 10
#define b01011 11
#define b01100 12
#define b01101 13
#define b01110 14
#define b01111 15
#define b10000 16
#define b10001 17
#define b10010 18
#define b10011 19
#define b10100 20
#define b10101 21
#define b10110 22
#define b10111 23
#define b11000 24
#define b11001 25
#define b11010 26
#define b11011 27
#define b11100 28
#define b11101 29
#define b11110 30
#define b11111 31


static uint8_t logo[] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00100,
	0b01110,
	0b11011,
	0b10001,
	0b00000,
	0b00100,
	0b01110,
	0b11011,
	0b10001,
	0b00000,
	0b00100,
	0b01110,
	0b11011,
	0b10001,
	0b00000,
	0b10000,
	0b11000,
	0b01100,
	0b00110,
	0b00011,
	0b00001,
	0b00000,
	0b10001,
	0b11011,
	0b01110,
	0b00100,
	0b00000,
	0b10001,
	0b11011,
	0b01110,
	0b00100,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};



static uint8_t tick() {
	static uint16_t t = 0;

	// draw row
	uint8_t x;
	for(x = 0; x < LED_WIDTH; x++) {
		uint8_t i = (t >> 3) + x;
		if(i >= sizeof(logo)) i -= sizeof(logo);

		uint8_t bits1 = logo[i];
		uint8_t bits2 = logo[i + 1 == sizeof(logo)? 0 : i + 1];

		uint8_t weight2 = t & 7;
		uint8_t weight1 = 7 - weight2;

		int8_t y;
		for(y = LED_HEIGHT - 1; y >= 0; y--) {

			uint8_t color = (-(bits1 & 1) & weight1) +
							(-(bits2 & 1) & weight2);

//			color &= 4 + (((x + y * 2 + t)>>2) & 3);	// apply magic


			setLedXY(x, y, color);

			bits1 >>= 1;
			bits2 >>= 1;
		}
	}

	// move
	if(++t == (sizeof(logo) << 3)) t = 0;
	return 0;
}

static void init(void) ATTRIBUTES;
void init(void) { registerAnimation(tick, 2, sizeof(logo) * 8); }

