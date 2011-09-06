#if defined(__AVR__)
#include "../main.h"
void init_plasma(void) __attribute__ ((naked, used, section (".init8")));
#warning compiling for AVR
#else
#include "main.h"
void init_plasma(void) __attribute__((constructor));
#warning compiling for SIM
#endif

static int16_t sini(uint8_t x) {
	static int16_t table[] = {
	0, 6, 13, 19, 25, 31, 37, 43, 49, 55, 60, 66, 71, 76, 81, 86, 91, 95, 99,
	103, 106, 110, 113, 116, 118, 121, 122, 124, 126, 127, 127, 128, 128 };
	x = x & 127;
	uint8_t i = x & 31;
	int16_t ret = (x & 32) ? table[32 - i] : table[i];
	if(x & 64) ret = -ret;
	return ret;
}

static uint8_t tick(void);

int a = 0;

uint8_t tick() {


	
	uint8_t x,y;
	
	
	for(y = 0; y < LED_HEIGHT; y++) {
		for(x = 0; x < LED_WIDTH; x++) {

			uint16_t b = 2*sini((x+(a))*2) + 3*sini((y+(a))*3) + 10 * sini(a);

			setLedXY(x, y, (b>>7)&3);

		}
	}
	a++;
	return 0;
	
}


void init_plasma(void) {
	registerAnimation(tick, 4,150);
}
           
           

