#include "sim.h"

static int sini(int x) {
	static const int table[] = {
	0, 6, 13, 19, 25, 31, 37, 43, 49, 55, 60, 66, 71, 76, 81, 86, 91, 95, 99,
	103, 106, 110, 113, 116, 118, 121, 122, 124, 126, 127, 127, 128, 128 };
	x = x & 127;
	int i = x & 31;
	int ret = (x & 32) ? table[32 - i] : table[i];
	if(x & 64) ret = -ret;
	return ret;
}
static inline int cosi(int x) { return sini(x + 32); }


void init() {
	setTickInterval(2);
}


int v1 = 0;
int v2 = 0;

void tick() {

	v1 += 3;
	v2 += 5;

	int rx = (cosi(v1 >> 2) >> 5) + 2;
	int ry = (sini(v2 >> 2) >> 5) + 2;

	int x, y;
	for(y = 0; y < LED_HEIGHT; y++) {
		for(x = 0; x < LED_WIDTH; x++) {

			int dx = x - rx;
			int dy = y - ry;
			int d = sqrt(dx * dx + dy * dy);

			int q = (
						(sini(x * 8 + (v1)) +
						sini(y * 8 + (v2)) +
						sini(d << 5))
					>> 2) + 128;

			int a = (q * 0xffff / 0x10000) >> 6;
			setLedXY(x, y, a);

		}
	}
}

