#include "sim.h"


void init() {
	setTickInterval(4);
}


int m = 0;
int c = 0;
int d = 1;


void tick() {
	int x;
	for(x = 0; x < LED_WIDTH * LED_HEIGHT; x++) setLedXY(x & 3, x >> 2, 0);

	m += d;
	if(m < 1 || m == LED_HEIGHT - 1) d = -d;

	for(x = 0; x < LED_WIDTH; x++) setLedXY(x, m, 3);
}

