#include <stdlib.h>
#include <main.h>


static int16_t sini(uint8_t x) {
	static int16_t table[] = {
	0, 6, 13, 19, 25, 31, 37, 43, 49, 55, 60, 66, 71, 76, 81, 86, 91, 95, 99,
	103, 106, 110, 113, 116, 118, 121, 122, 124, 126, 127, 127, 128, 128 };
	x = x & 127;
	uint8_t i = x & 31;
	int16_t ret = (x & 32) ? table[32 - i] : table[i];
	if(x & 64) ret = -ret;
	return ret;
}
static int16_t cosi(uint8_t x) { return sini(x + 32); }


static uint8_t tick() {
	static int _a = 0;
	static int b = 0;
	static uint8_t init = 0;

	if(init == 0) {
		_a = rand();
		b = rand();
		init = 1;
	}

	int a = _a >> 1;

	uint8_t x, y;
	for(y = 0; y < LED_HEIGHT; y++) {
		for(x = 0; x < LED_WIDTH; x++) {

			uint16_t c = 5*sini(x*2+sini(a)) + 8*cosi(y*3+a+120) + 3*sini(b/2);
			uint16_t d = 2*sini(x*2-b) + 3*cosi(y*3-cosi(a)) + 3*sini(a/4)+ c;

			uint8_t col = (((cosi(d>>4)>>5))+(d>>6))&15;
			
			if(col > 7) col = 15 - col;

			setLedXY(x, y, col);
		}
	}
	_a--;
	b = cosi(a);
	return 0;
	
}

static void init(void) ATTRIBUTES;
void init(void) {
	registerAnimation(tick, 6, 150);
}
           
           

