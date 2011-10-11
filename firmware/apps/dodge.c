#include <main.h>


static int done = 0;

static uint8_t tick() {

	static int8_t m = 0;
	static int8_t d = 1;

	int8_t x;
	// clear
	for(x = 0; x < LED_WIDTH * LED_HEIGHT; x++) setLedXY(x & 3, x >> 2, 0); // < looks nice, but does not work when LED_WIDTH/HEIGHT changes

	m += d;
	if(m < 1 || m == LED_HEIGHT - 1) d = -d;

	// draw row
	for(x = 0; x < LED_WIDTH; x++) setLedXY(x, m, 7);

	return done;
}


static void key(key_type key, event_type event) {

	if(event == DOWN)
		if(key == KEY_A) done = 1;
}


static void init(void) ATTRIBUTES;
void init(void) {
	registerApp(tick, key, 22);
}


