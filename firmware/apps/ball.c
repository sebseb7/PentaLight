#include <stdlib.h>
#include <stdio.h>

#include <main.h>

void init_ball(void) ATTRIBUTES;

static uint8_t tick_ball(void);

// an extension to the bouncing box
#define BOFF		-1

int8_t x = 2, y = 2;
int8_t dx = 1, dy = 1;

void init_ball(void) {
	registerAnimation(tick_ball, 6, 40);
}

uint8_t tick_ball() {
	// bounce

	if(x == BOFF || x == LED_WIDTH - BOFF - 1) {
		dx *= -1;
	}

	if(y == BOFF || y == LED_HEIGHT - BOFF - 1) {
		dy *= -1;
	}

	// move

	x += dx;
	y += dy;

	// display

	const uint8_t ball[3][3] = {
		{2, 5, 2},
		{5, 7, 5},
		{2, 5, 2},
		};

	printf("tick\n");

	int8_t tx, ty;
	for(tx = -2; tx <= 2; ++tx) {
		for(ty = -2; ty <= 2; ++ty) {
			const int8_t ax = tx + x,
			      ay = ty + y;

			if(ax >= 0 && ay >= 0 && ax < LED_WIDTH && ay < LED_HEIGHT) {
				uint8_t bright = 0;

				if(abs(tx) <= 1 && abs(ty) <= 1) {
					bright = ball[tx+1][ty+1];
				}

				setLedXY(ax, ay, bright);
			}
		}
	}

}

