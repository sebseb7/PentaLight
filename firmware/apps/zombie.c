#include <main.h>

#include <stdlib.h>

#define MAX_ZOMBIES		5
#define MAX_PROGRESS		100
#define ZOMBIE_SLOW		2

#define ABS(x)		(x > 0 ? x : -x)

static int8_t player[3];
static int8_t zombies[MAX_ZOMBIES][3];
static int8_t zombie_set;
static uint8_t progress;

static uint8_t zombie_tick = 0;
static int8_t state = -1;

static void init(void) ATTRIBUTES;

static uint8_t tick(void);
static void key(key_type key, event_type event);

static int8_t move(int8_t *entity) {
	switch(entity[2]) {
		case 0:
			++entity[0];
			break;
		case 1:
			++entity[1];
			break;
		case 2:
			--entity[0];
			break;
		case 3:
			--entity[1];
			break;
	}

	return entity[0] >= 0 && entity[0] < LED_WIDTH &&
		entity[1] >= 0 && entity[1] < LED_HEIGHT;
}

void init(void) {
	registerApp(tick, key, 30);
}

uint8_t tick(void) {
	puts("tick");

	// INIT
	if(state < 0) {
		player[0] = 2;
		player[1] = 0;
		player[2] = 1;

		zombie_set = 0;

		progress = 0;

		state = 0;
	}

	// RUN
	if(state == 0) {
		setLedXY(player[0], player[1], 0);

		if(!move(player)) {
			puts("bump");
			state = 1;
		}

		int8_t zombie_count = 0;
		int8_t free_zombie;
		for(uint8_t i; i < MAX_ZOMBIES; ++i) {
			int8_t bit = 1 << i;

			if(zombie_set & bit) {
				if(zombie_tick == 0) {
					setLedXY(zombies[i][0], zombies[i][1], 0);

					if(!move(zombies[i])) {
						zombie_set &= ~bit;
					} else {
						setLedXY(zombies[i][0], zombies[i][1], 5);
					}
				}

				zombie_count++;

				if(zombies[i][0] == player[0] && zombies[i][1] == player[1]) {
					state = 1;
				}
			} else {
				free_zombie = i;
			}
		}

		int8_t target_zombies = (int16_t) progress * MAX_ZOMBIES / MAX_PROGRESS;
		if(zombie_count < target_zombies) {
			int8_t fits;
			int8_t *zombie = zombies[free_zombie];

			puts("spawning zombie");

			zombie_set |= 1 << free_zombie;

			// TODO: spawn
			do {
				zombie[2] = rand() % 4;

				if(zombie[2] & 1) {
					zombie[0] = rand() % LED_WIDTH;
					zombie[1] = zombie[2] == 1 ? 0 : LED_HEIGHT - 1;
				} else {
					zombie[0] = zombie[2] == 0 ? 0 : LED_WIDTH - 1;
					zombie[1] = rand() % LED_HEIGHT;
				}

				int8_t dist = abs(zombie[0] - player[0]) + abs(zombie[1] - player[1]);
				fits = dist > 2;
			} while(!fits);

			setLedXY(zombie[0], zombie[1], 5);
		}

		setLedXY(player[0], player[1], 7);

		if(progress < MAX_PROGRESS) {
			++progress;
		}

		++zombie_tick;
		if(zombie_tick >= ZOMBIE_SLOW) {
			zombie_tick = 0;
		}
	}

	// THE END
	if(state > 0) {
		state = -1;
		return 1;
	}

	return 0;
}

void key(key_type key, event_type event) {
	if(event == DOWN) {
		if(key == 1) {
			player[2] = player[2] == 3 ? 0 : player[2] + 1;
		}

		if(key == 0) {
			player[2] = player[2] == 0 ? 3 : player[2] - 1;
		}
	}
}

