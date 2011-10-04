#include <main.h>

static void init_test(void) ATTRIBUTES;

static int8_t d = 1;
static int8_t e = 1;


static uint8_t tick_test(void);

static void init_test(void)
{
	registerAnimation(tick_test,5,60);
}
            


uint8_t tick_test() 
{
	

	for(uint8_t x = 0; x < LED_WIDTH * LED_HEIGHT; x++) setLedXY(x & 3, x >> 2, 7*(d&1));

	d++; 
	return 0;
}

