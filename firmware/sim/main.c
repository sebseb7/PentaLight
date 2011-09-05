#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <SDL/SDL.h>

#include "main.h"


#define ZOOM 20


int leds[LED_HEIGHT][LED_WIDTH];
int interval;
uint8_t (*tick_fp)(void);


void setLedXY(uint8_t x, uint8_t y, uint8_t b) {
	assert(x < LED_WIDTH);
	assert(y < LED_HEIGHT);
	assert(b < 4);
	leds[y][x] = b;
}


void registerAnimation(uint8_t (*fp)(void),uint16_t t,uint16_t ignore)
{
    tick_fp = fp;
    
	assert(t > 0);
	interval = 1000000 / (244 >> t);
}
    


int main(int argc, char *argv[]) {

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

		tick_fp();

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




