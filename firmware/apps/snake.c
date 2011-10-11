#include <main.h>

#include <stdlib.h>

#define CYCLE(x,c)		((x + c) % c)
#define ARRAY_SIZE(array)	(sizeof(array) / sizeof(*array))

#define MAX_LENGTH		(LED_WIDTH * LED_HEIGHT)

#define MAX_TICK		2

static void init_snake(void) ATTRIBUTES;

static uint8_t tick_snake(void);
static void key_snake(key_type key, event_type event);

static int8_t direction = 0;
static int8_t buffer[MAX_LENGTH * 2] = {1, 2, 2, 2};
static int8_t pointer = 2;
static int8_t length = 1;
static int8_t apple[2];
static int8_t end = -1;
static uint8_t tick = 0;

static int8_t collision(const int8_t x, const int8_t y, const int8_t off) {
	for(int8_t i = off; i < length; ++i) {
		const int8_t cur = CYCLE(pointer - i * 2, ARRAY_SIZE(buffer));

		if(buffer[cur] == x && buffer[cur + 1] == y) {
			return 1;
		}
	}

	return 0;
}

static void new_apple(void) {
	if(length == LED_WIDTH * LED_HEIGHT) {
		end = 1;
		return;
	}

	for(;;) {
		apple[0] = rand() % LED_WIDTH;
		apple[1] = rand() % LED_HEIGHT;

		if(!collision(apple[0], apple[1], 0)) {
			break;
		}
	}
}

void init_snake(void) {
	registerApp(tick_snake, key_snake, 22);
}

uint8_t tick_snake() {
	int8_t x = buffer[pointer],
	       y = buffer[pointer + 1];

	// init?

	if(end < 0) {
		new_apple();
		end = 0;
	}

	// the end?

	if(end > 0) {
		for(int8_t i = length - 1; i >= 0; --i) {
			const int8_t cur = CYCLE(pointer - i * 2, ARRAY_SIZE(buffer));

			setLedXY(buffer[cur], buffer[cur + 1], (end % 3) * 2 + (i == 0) * 3);
		}

		++end;

		if(end > 9) {
			end = -1;
			length = 1;

			return 1;
		} else {
			return 0;
		}
	}

	// game step or animation?

	if(tick == 0) {
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

		// save

		pointer = (pointer + 2) % ARRAY_SIZE(buffer);
		buffer[pointer] = x;
		buffer[pointer + 1] = y;

		// eaten?

		if(apple[0] == x && apple[1] == y) {
			++length;
			new_apple();
		}

		// collision

		if(collision(x, y, 1)) {
			end = 1;
		}

		// paint start of tail

		const int8_t last = CYCLE(pointer - 2, ARRAY_SIZE(buffer));
		setLedXY(buffer[last], buffer[last+1], 3);

		// clear tail

		const int8_t end = CYCLE(pointer - length * 2, ARRAY_SIZE(buffer));
		setLedXY(buffer[end], buffer[end + 1], 0);

		// paint apple

		setLedXY(apple[0], apple[1], MAX_TICK);

		// paint head

		setLedXY(x, y, 7);
	} else {
		// pulsing apple
		setLedXY(apple[0], apple[1], tick);
	}

	// tick on

	if(++tick >= MAX_TICK) {
		tick = 0;
	}

	return 0;
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

