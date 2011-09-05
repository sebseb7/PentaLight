#include "main.h"


void init() {
	setTickInterval(4);
}


int8_t m = 0;
int8_t d = 1;


void tick() {
	int8_t x;
	// clear
	for(x = 0; x < LED_WIDTH * LED_HEIGHT; x++) setLedXY(x & 3, x >> 2, 0);

	m += d;
	if(m < 1 || m == LED_HEIGHT - 1) d = -d;

	// draw row
	for(x = 0; x < LED_WIDTH; x++) setLedXY(x, m, 3);
}

