#if defined(__AVR__)
#include "../main.h"
void init_test(void) __attribute__ ((naked, used, section (".init8")));
#warning compiling for AVR
#else
#include "main.h"
void init_test(void) __attribute__((constructor));
#warning compiling for SIM
#endif



int8_t m = 0;
int8_t d = 1;


void tick_test(void);


void init_test(void)
{
    void (*fp)(void);
	fp=tick_test;
        
	registerAnimation(fp,4);
}
            


void tick_test() {
	int8_t x;
	// clear
	for(x = 0; x < LED_WIDTH * LED_HEIGHT; x++) setLedXY(x & 3, x >> 2, 0);

	m += d;
	if(m < 1 || m == LED_HEIGHT - 1) d = -d;

	// draw row
	for(x = 0; x < LED_WIDTH; x++) setLedXY(x, m, 3);
}

