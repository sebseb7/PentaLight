#include <main.h>

void init_test(void) ATTRIBUTES;

static const uint8_t leds[5][4] = {
	{  1,  2,  3,  4 },
	{ 14, 15, 16,  5 },
	{ 13, 20, 17,  6 },
	{ 12, 19, 18,  7 },
	{ 11, 10,  9,  8 },
};

static int8_t t = 0;
static int8_t d = 1;

static uint8_t tick(void) {
	int x, y;

	t++;
	t &= 0x7f;
	if(t == 0) d = -d;

	for(x = 0; x < 4; x++)
		for(y = 0; y < 5; y++)
			setLedXY(x, y, ((leds[y][x] + t * d) >> 2) & 7);

	return 0;
}


void init_test(void) {
	registerAnimation(tick, 3, 0x7f * 2);
}


