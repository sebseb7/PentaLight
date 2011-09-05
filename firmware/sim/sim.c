#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <SDL/SDL.h>
#include "sim.h"


#define ZOOM 20


int leds[LED_HEIGHT][LED_WIDTH];
int interval;

void setTickInterval(int t) {
	assert(t > 0);
	interval = 1000000 / (244 >> t);
}

void setLedXY(int x, int y, int b) {
	assert(x >= 0 && x < LED_WIDTH);
	assert(y >= 0 && y < LED_HEIGHT);
	assert(b >= 0 && b < 4);
	leds[y][x] = b;
}



int main(int argc, char *argv[]) {

	setTickInterval(1);
	init();

	SDL_Surface* screen = SDL_SetVideoMode(LED_WIDTH * ZOOM, LED_HEIGHT * ZOOM,
		32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	int running = 1;
	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_KEYDOWN:
					if(ev.key.keysym.sym == SDLK_ESCAPE) running = 0;
					break;
				case SDL_KEYUP:
					break;
				default: break;
			}
		}

		tick();

		const unsigned int color[] = {
			0x000000, 0x7f0000, 0xcf0000, 0xff3f3f,
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




