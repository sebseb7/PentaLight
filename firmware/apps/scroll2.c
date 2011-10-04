#include <main.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

#include "../libs/font.h"


void init_scroll2(void) ATTRIBUTES;


#ifdef __AVR__
char text2[] PROGMEM =" <<</>> Pentalight ";
#else
char text2[] =" <<</>> Pentalight ";
#endif

const uint8_t text_len2 = 19;

uint16_t pos2 = 0;

static uint8_t tick2(void) {
	uint8_t x, y;

	for(x = 0; x < LED_WIDTH; x++) {
		uint16_t p = pos2 + x;
#ifdef __AVR__
		char c = pgm_read_byte(&text2[p / 4]);
#else
		char c = text2[p / 4];
#endif
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


