#if defined(__AVR__)
#include "../main.h"
void init_plasma(void) __attribute__ ((naked, used, section (".init8")));
#warning compiling for AVR
#else
#include "main.h"
void init_plasma(void) __attribute__((constructor));
#warning compiling for SIM
#endif

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

static uint8_t sqrti(uint8_t x) {
	static const uint8_t table[32] = {
	0, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5 };
	return table[x];
}

uint8_t plasma_tick(void);

void init_plasma(void)
{
	registerAnimation(plasma_tick,2,40);
}
           
           

int v1 = 0;
int v2 = 0;

uint8_t plasma_tick() {

	v1 += 3;
	v2 += 5;

	int rx = (cosi(v1 >> 2) >> 5) + 2;
	int ry = (sini(v2 >> 2) >> 5) + 2;

	int x, y;
	for(y = 0; y < LED_HEIGHT; y++) {
		for(x = 0; x < LED_WIDTH; x++) {

			int dx = x - rx;
			int dy = y - ry;
			int d = sqrti(dx * dx + dy * dy);

			int q = (	(sini(x * 8 + (v1)) +
						sini(y * 8 + (v2)) +
						sini(d << 5)) >> 2) + 128;

			int a = (q * 0xffff / 0x10000) >> 6;
			setLedXY(x, y, a);
		}
	}
	return 0;
	
}


void init_plasma(void) {
	registerAnimation(tick, 2);
}
           
           

