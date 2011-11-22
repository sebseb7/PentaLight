#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "main.h"
#include "sdl_draw/SDL_draw.h"

#define ZOOM 50

int leds[LED_HEIGHT][LED_WIDTH];
int interval;


uint16_t volatile current_adc_value = 900;

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
	// 122Hz / tick
	interval = 1000000 / 122 * t;
}

void registerApp(tick_fun tick, key_fun key, uint16_t t)
{
	tick_fp = tick;
	key_fp = key;

	assert(t > 0);
	interval = 1000000 / 122 * t;
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
	srand(time(NULL));

	SDL_Surface* screen = SDL_SetVideoMode(LED_WIDTH * ZOOM, LED_HEIGHT * ZOOM,
		32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	SDL_Rect rect = { 0, 0, LED_WIDTH*ZOOM, LED_HEIGHT*ZOOM };
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0x20,0x20,0x60));

	unsigned int color[] = {
		SDL_MapRGB(screen->format, 0x57,0x10,0x10),
		SDL_MapRGB(screen->format, 0x6f,0x20,0x20),
		SDL_MapRGB(screen->format, 0x87,0x30,0x30),
		SDL_MapRGB(screen->format, 0x9f,0x40,0x40),
		SDL_MapRGB(screen->format, 0xb7,0x50,0x50),
		SDL_MapRGB(screen->format, 0xcf,0x60,0x60),
		SDL_MapRGB(screen->format, 0xe7,0x70,0x70),
		SDL_MapRGB(screen->format, 0xff,0x80,0x80)
	};

	int running = 1;
	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = 0;
					break;

                case 'd':
				case SDLK_RIGHT:
					key_emit(KEY_A, ev.type);
					break;

                case 'a':
				case SDLK_LEFT:
					key_emit(KEY_B, ev.type);
					break;

				default: break;
			}
		}

		running &= !tick_fp();

		int x, y;
		for(x = 0; x < LED_WIDTH; x++) {
			for(y = 0; y < LED_HEIGHT; y++) {
				Draw_FillCircle(screen, ZOOM*x+(ZOOM/2),ZOOM*(LED_HEIGHT - y - 1)+(ZOOM/2), (ZOOM*0.35), color[leds[y][x]]);
			}
		}
		SDL_Flip(screen);

		usleep(interval);
	}

	SDL_Quit();
	return 0;
}

