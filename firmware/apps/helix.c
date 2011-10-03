#include <main.h>

#define SUB_ANIMS 2

static const uint8_t map[][5][4] = {
	{
		{  1,  2,  3,  4 },
		{ 14, 15, 16,  5 },
		{ 13, 20, 17,  6 },
		{ 12, 19, 18,  7 },
		{ 11, 10,  9,  8 },
	},
	{
		{  5,  6, 15, 16 },
		{  4,  7, 14, 17 },
		{  3,  8, 13, 18 },
		{  2,  9, 12, 19 },
		{  1, 10, 11, 20 },
	},

};


static uint8_t tick(void) {

	static int8_t count = 0;
	static int8_t flip = 0;
	static uint8_t anim = 0;

	count++;
	count &= 0x7f;
	if(count == 0) {
		if(flip) {	// next anim
			anim++;
			anim %= SUB_ANIMS;
		}
		flip = !flip;
	}

	int8_t q = flip ? -count : count;

	int x, y;
	for(x = 0; x < 4; x++)
		for(y = 0; y < 5; y++)
			setLedXY(x, y, ((map[anim][y][x] + q) >> 2) & 7);

	return 0;
}


static void init(void) ATTRIBUTES;
void init(void) {
	registerAnimation(tick, 5, 0x7f * 2 * SUB_ANIMS);
}


