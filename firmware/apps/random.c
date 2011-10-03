#include <stdlib.h>

#include <main.h>

void init_random(void) ATTRIBUTES;

static uint8_t tick_random(void);

void init_random(void)
{
	registerAnimation(tick_random,16,60);
}
            


uint8_t tick_random() {
	uint8_t x, y;

	for(x = 0; x < LED_WIDTH; x++) {
		for(y = 0; y < LED_HEIGHT; y++) {
			setLedXY(x, y, rand()&7);
		}
	}
	
	return 0;
}
