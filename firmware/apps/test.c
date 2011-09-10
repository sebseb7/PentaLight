#include <main.h>

void init_test(void) ATTRIBUTES;

int8_t m = 0;
int8_t d = 1;


static uint8_t tick_test(void);

void init_test(void)
{
	registerAnimation(tick_test,4,40);
}
            


uint8_t tick_test() {
	int8_t x;
	// clear
	for(x = 0; x < LED_WIDTH * LED_HEIGHT; x++) setLedXY(x & 3, x >> 2, 0); // < looks nice, but does not work when LED_WIDTH/HEIGHT changes

	m += d;
	if(m < 1 || m == LED_HEIGHT - 1) d = -d;

	// draw row
	for(x = 0; x < LED_WIDTH; x++) setLedXY(x, m, 7);
	
	return 0;
}

