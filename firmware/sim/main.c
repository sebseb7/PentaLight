#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <SDL/SDL.h>

#include "main.h"

#define ZOOM 50

int leds[LED_HEIGHT][LED_WIDTH];
int interval;

tick_fun tick_fp;
key_fun key_fp;

void setLedXY(uint8_t x, uint8_t y, uint8_t b) {
	assert(x < LED_WIDTH);
	assert(y < LED_HEIGHT);
	assert(b < 8);
	leds[y][x] = b;
}

void registerAnimation(tick_fun tick, uint16_t t, uint16_t ignore)
{
	tick_fp = tick;
	key_fp = NULL;
    
	assert(t > 0);
	interval = 1000000 / (244 >> t);
}
    
void registerApp(tick_fun tick, key_fun key, uint16_t t)
{
	tick_fp = tick;
	key_fp = key;
    
	assert(t > 0);
	interval = 1000000 / (244 >> t);
}

void key_emit(key_type key, SDL_EventType type) {
	if(key_fp == NULL) {
		return;
	}

	event_type event;

	if(type == SDL_KEYDOWN) {
		event = DOWN;
	} else if(type == SDL_KEYUP) {
		event = UP;
	} else {
		return;
	}

	key_fp(key, event);
}

int main(int argc, char *argv[]) {

	SDL_Surface* screen = SDL_SetVideoMode(LED_WIDTH * ZOOM, LED_HEIGHT * ZOOM,
		32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	int running = 1;
	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = 0;
					break;

				case SDLK_RIGHT:
					key_emit(KEY_A, ev.type);
					break;

				case SDLK_LEFT:
					key_emit(KEY_B, ev.type);
					break;

				default: break;
			}
		}

		tick_fp();

		const unsigned int color[] = {
	//		 0x000000, 0x7f0000, 0xcf0000, 0xff3f3f,
			0x000000, 0x240000, 0x480000,0x6c0000,0x900000,0xb40000,0xd90000, 0xff3f3f,
		};
		int x, y;
		for(x = 0; x < LED_WIDTH; x++) {
			for(y = 0; y < LED_HEIGHT; y++) {
				SDL_Rect rect = { x * ZOOM, (LED_HEIGHT - y - 1) * ZOOM, ZOOM, ZOOM };
				SDL_FillRect(screen, &rect, color[leds[y][x]]);
			}
		}
		SDL_Flip(screen);

		usleep(interval);
	}

	SDL_Quit();
	return 0;
}

