#include <stdint.h>
#include <stdlib.h>


// #ifdef __AVR__
// #endif

#include "main.h"


void init() {
	setTickInterval(4);
}

void tick() {
	uint8_t x, y;

	for(x = 0; x < LED_WIDTH; x++) {
		for(y = 0; y < LED_HEIGHT; y++) {
			setLedXY(x, y, rand()&3);
		}
	}
}
