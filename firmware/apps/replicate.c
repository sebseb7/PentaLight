#include <main.h>
#include <stdlib.h>
#include "../libs/font.h"

#define		MAX_CHAIN_LENGTH		128

typedef enum { INIT, SHOW, BREAK, LISTEN, SCORE, QUIT } State;

static State state = INIT;
static uint8_t chain[MAX_CHAIN_LENGTH];
static uint8_t length;
static uint8_t step;
static uint8_t sub_step;

static char score[5] = { ' ', ' ', '0', '0', '0' };

static uint8_t tick() {
	uint8_t x, y;

	switch(state) {
		case INIT:
			length = 1;
			step = 0;
			sub_step = 0;
			state = SHOW;
			for(x = 0; x < MAX_CHAIN_LENGTH; x++) chain[x] = rand() & 1;

		case SHOW: {

			uint8_t side = chain[step];
			uint8_t color = sub_step < 7 ? 7 - sub_step : 0;

			for(x = 0; x < LED_WIDTH; ++x) {
				uint8_t c = (x >= LED_WIDTH / 2) ^ side ? color : 0;
				for(y = 0; y < LED_HEIGHT; ++y)
					setLedXY(x, y, c);
			}

			++sub_step;
			if(sub_step == 10) {
				sub_step = 0;
				++step;
				if(step == length) {
					step = 0;
					state = LISTEN;
				}
			}
			break;
		}

		case BREAK:
			++sub_step;
			if(sub_step == 5) {
				sub_step = 0;
				state = SHOW;
			}
			break;

		case SCORE:
			for(x = 0; x < LED_WIDTH; ++x) {

				uint16_t p = (sub_step / 4) + x;
				char c = score[p / 4 % 5];
				uint8_t bits = 0;
#ifdef __AVR__
				if((p & 3) < 3) bits = pgm_read_byte(&font[c - 32][p & 3]);
#else
				if((p & 3) < 3) bits =                font[c - 32][p & 3] ;
#endif
				for(y = 0; y < LED_HEIGHT; ++y) {
					setLedXY(x, y, 7 * (bits & 1));
					bits >>= 1;
				}
			}
			++sub_step;
			break;

		case QUIT:
			state = INIT;
			return 1;

		default:
			break;
	}
	return 0;
}

static void key(key_type key, event_type event) {
	if(event != DOWN) return;
	if(state == SCORE) state = QUIT;
	if(state != LISTEN) return;

	if(key == chain[step]) {
		++step;
		sub_step = 0;
		if(step == length) {

			// wow, you just beat the game!
			if(length == MAX_CHAIN_LENGTH) goto DO_SCORE;
			step = 0;
			++length;
			state = BREAK;
		}
	}
	else {
		DO_SCORE:
		state = SCORE;
		sub_step = 12;
		uint8_t i;
		for(i = 4; i > 1; --i) {
			score[i] = '0' + length % 10;
			length /= 10;
		}
	}
}

static void init(void) ATTRIBUTES;
void init(void) {
	registerApp(tick, key, 7);
}

