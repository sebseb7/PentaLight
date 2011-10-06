#include <main.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

#include "../libs/font.h"


static void init_scroll2(void) ATTRIBUTES;


static char text2[] =" 0,0V ";

static const uint8_t text_len2 = 6;

static uint16_t pos2 = 0;

static uint8_t tick2(void) {
	uint8_t x, y;

	if(pos2==0)
	{
		// this is not very accurate (3.1 would be better), but this would take 550 bytes for float emulation
		uint16_t vcc = current_adc_value / 3;
		
		uint8_t ch1=0;
		uint8_t ch2=0;
		
		while(vcc>100)
		{
			vcc-=100;
			ch1++;
		}
		while(vcc>10)
		{
			vcc-=10;
			ch2++;
		}
		text2[1]=ch1+16+32;
		text2[3]=ch2+16+32;
		
	}



	for(x = 0; x < LED_WIDTH; x++) {
		uint16_t p = pos2 + x;
		char c = text2[p / 4];

		uint8_t bits = 0;
#ifdef __AVR__
		if((p & 3) < 3) bits =  pgm_read_byte(&font[c - 32][p & 3]);
#else
		if((p & 3) < 3) bits =  			   font[c - 32][p & 3];
#endif

		for(y = 0; y < LED_HEIGHT; y++) {
			setLedXY(x, y, 7 * (bits & 1));
			bits >>= 1;
		}
	}
	pos2++;
	if(pos2 + LED_WIDTH == text_len2 * 4) pos2 = 0;
	
	return 0;
}


void init_scroll2(void) {
	registerAnimation(tick2, 16, (text_len2-1) * 4);
}


