#include <main.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

#include "../libs/font.h"


void init_scroll(void) ATTRIBUTES;


#ifdef __AVR__
char text[] PROGMEM =" Cloudy mit Aussicht auf Datenspuren ";
#else
char text[] =" Cloudy mit Aussicht auf Datenspuren ";
#endif

const uint8_t text_len = 36;

uint16_t pos = 0;

static uint8_t tick(void) {
	uint8_t x, y;

	for(x = 0; x < LED_WIDTH; x++) {
		uint16_t p = pos + x;
#ifdef __AVR__
		char c = pgm_read_byte(&text[p / 4]);
#else
		char c = text[p / 4];
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
	pos++;
	if(pos + LED_WIDTH == text_len * 4) pos = 0;
	
	return 0;
}


void init_scroll(void) {
	registerAnimation(tick, 5, text_len * 4);
}


