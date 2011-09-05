#include <stdlib.h>

#if defined(__AVR__)
#include "../main.h"
void init_random(void) __attribute__ ((naked, used, section (".init8")));
#warning compiling for AVR
#else
#include "main.h"
void init_random(void) __attribute__((constructor));
#warning compiling for SIM
#endif



void tick_random(void);

void init_random(void)
{
	registerAnimation(tick_random,4);
}
            


void tick_random() {
	uint8_t x, y;

	for(x = 0; x < LED_WIDTH; x++) {
		for(y = 0; y < LED_HEIGHT; y++) {
			setLedXY(x, y, rand()&3);
		}
	}
}
