#include <main.h>

#include <stdio.h>

#define CYCLE(x,c)		((x + c) % c)

void init_snake(void) ATTRIBUTES;

static uint8_t tick_snake(void);
static void key_snake(key_type key, event_type event);

int8_t x = 2, y = 2;
int8_t direction = 0;

void init_snake(void) {
	registerApp(tick_snake, key_snake, 5);
}

uint8_t tick_snake() {
	// clear

	setLedXY(x, y, 0);

	// move

	switch(direction) {
		case 0:
			++x;
			break;
		case 1:
			++y;
			break;
		case 2:
			--x;
			break;
		case 3:
			--y;
			break;
	}

	x = CYCLE(x, LED_WIDTH);
	y = CYCLE(y, LED_HEIGHT);

	printf("%i %i\n", x, y);

	// paint

	setLedXY(x, y, 7);
}

void key_snake(key_type key, event_type event) {
	if(event == DOWN) {
		if(key == KEY_A) {
			--direction;
		} else {
			++direction;
		}
	}

	direction = CYCLE(direction, 4);
}

