#include <main.h>
#include <stdlib.h>

#define		CHAIN_LENGTH		128

typedef enum { INIT, SHOW, LISTEN, QUIT } State;

static State state = INIT;
static uint8_t chain[CHAIN_LENGTH];
static uint8_t length = 3;
static uint8_t level = 0;
static uint8_t step = 0;
static uint8_t sub_step = 0;

static uint8_t tick() {

	if(state == INIT) {
		uint8_t i;
		for(i = 0; i < length; i++) chain[i] = rand() & 1;

		state = SHOW;
	}

	if(state == SHOW) {
		++sub_step;
		if(sub_step < 10) {

			uint8_t side = chain[step];
			uint8_t color = sub_step < 8 ? 8 - sub_step : 0;

			uint8_t x, y;
			for(x = 0; x < LED_WIDTH; x++) {
				uint8_t c = (x >= LED_WIDTH / 2) ^ side;
				for(y = 0; y < LED_HEIGHT; y++)
				{
					setLedXY(x, y, c ? color : 0);
				}
			}
		}
		else {
			sub_step = 0;
			++step;
			if(step == length) {
				step = 0;
				state = LISTEN;
			}
		}

	}

	return state == QUIT;
}

static void key(key_type key, event_type event) {
	if(state != LISTEN || event != DOWN)
		return;

	if(key == chain[step]) {
		++step;
		if(step == length) {
			chain[step] = rand() & 1;
			step = 0;
			++length;
			++level;
			state = SHOW;
		}
	}
	else {
		// TODO: print level before exiting
		state = QUIT;
	}
}

static void init(void) ATTRIBUTES;
void init(void) {
	registerApp(tick, key, 7);
}


