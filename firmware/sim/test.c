#include "main.h"



int8_t m = 0;
int8_t d = 1;


void tick(void);

#if defined(__AVR__)
void init(void) __attribute__ ((naked, used, section (".init8")));
#warning compiling for AVR
#else
void init(void) __attribute__((constructor));
#warning compiling for SIM
#endif

void init(void)
{
    void (*fp)(void);
	fp=tick;
        
	registerAnimation(fp,4);
}
            


void tick() {
	int8_t x;
	// clear
	for(x = 0; x < LED_WIDTH * LED_HEIGHT; x++) setLedXY(x & 3, x >> 2, 0);

	m += d;
	if(m < 1 || m == LED_HEIGHT - 1) d = -d;

	// draw row
	for(x = 0; x < LED_WIDTH; x++) setLedXY(x, m, 3);
}

