#include <main.h>

#define MAX_SLOW	15
#define MIN_SLOW	5
#define SHOT_SLOW	10
#define SPEEDUP_SLOW	2

#define EXPL_LEN	20

#define ABS(x)		(x > 0 ? x : -x)

struct player {
	int8_t pos;
	int8_t dir;
	int8_t slow;
	int8_t tick;
	int8_t shot[2];
};

static void init(void) ATTRIBUTES;

static uint8_t tick(void);
static void key(key_type key, event_type event);

static int8_t state = -1;
static int8_t key_state;
static struct player players[2];
static int8_t lost;
static int8_t shot_tick;
static int8_t speedup_tick;

void init(void) {
	registerApp(tick, key, 5);
}

uint8_t tick(void) {
	// INIT
	if(state < 0) {
		// no key active
		key_state = 0;

		// nobody lost, yet
		lost = 0;

		// players start slowly in their corner
		for(uint8_t p = 0; p < 2; ++p) {
			struct player *player = &players[p];

			player->pos = p ? (LED_WIDTH - 1) : 0;
			player->dir = p ? -1 : 1;
			player->slow = MAX_SLOW;
			player->tick = MAX_SLOW;

			player->shot[0] = -1;
		}

		state = 0;
	}

	// RUN
	if(state == 0) {
		// player loop
		for(uint8_t p = 0; p < 2; ++p) {
			struct player *player = &players[p];
			const int8_t vert = p ? (LED_HEIGHT - 1) : 0;

			// player step
			if(++player->tick > player->slow) {
				// clear old position
				setLedXY(player->pos, vert, 0);

				// bounce?
				if(player->pos == (p ? 0 : LED_WIDTH - 1)) {
					// player->pos *= -1;
					player->pos = p ? (LED_WIDTH) : -1;
				}

				// move
				player->pos += player->dir;

				// paint new position
				setLedXY(player->pos, vert, 7);

				// reset the state
				player->tick = 0;
			}

			// shooting?
			if(key_state & (p + 1)) {
				if(player->shot[0] == -1) {
					// place projectile
					player->shot[1] = p ? (LED_HEIGHT - 1) : 0;
					player->shot[0] = player->pos;
				}

				// slowdown
				player->slow = MIN_SLOW;
			}

			// speedup
			if(speedup_tick == 0 && player->slow < MAX_SLOW) {
				++player->slow;
			}
		}

		if(++speedup_tick > SPEEDUP_SLOW) {
			speedup_tick = 0;
		}

		// shot loop
		for(uint8_t p = 0; p < 2; ++p) {
			int8_t *shot = players[p].shot;

			if(shot[0] >= 0) {
				if(++shot_tick > SHOT_SLOW) {
					// clear projectile?
					if(shot[1]) {
						setLedXY(shot[0], shot[1], 0);
					}

					// move projectile?
					shot[1] += p ? -1 : 1;


					if(shot[1] < 0 || shot[1] >= LED_HEIGHT) {
						shot[0] = -1;
					} else {
						// paint projectile
						setLedXY(shot[0], shot[1], 3);
					}

					// reset
					shot_tick = 0;
				}

				// hit?
				if(shot[1] == (p ? 0 : (LED_HEIGHT - 1)) && shot[0] == players[!p].pos) {
					state = 1;
					lost |= !p + 1;
				}
			}
		}
	}

	// THE END
	if(state > 0) {
		int8_t expand = state < EXPL_LEN ? state : (2 * EXPL_LEN - state);

		for(uint8_t p = 0; p < 2; ++p) {
			if((p + 1) & lost) {
				for(uint8_t x = 0; x < LED_WIDTH; ++x) {
					for(uint8_t y = 0; y < LED_HEIGHT; ++y) {
						const int16_t dx = x - players[p].pos;
						const int16_t dy = y - (p ? LED_HEIGHT - 1 : 0);

						int16_t bright = (7 - (dx * dx + dy * dy)) * expand / EXPL_LEN;

						if(bright >= 0) {
							setLedXY(x, y, bright);
						}
					}
				}
			}
		}

		for(uint8_t p = 0; p < 2; ++p) {
			setLedXY(players[p].pos, p ? LED_HEIGHT - 1 : 0, 7);
		}

		if(state >= EXPL_LEN * 2) {
			state = -1;
			return 1;
		} else {
			state++;
		}
	}

	return 0;
}

void key(key_type key, event_type event) {
	if(event == DOWN) {
		key_state |= key + 1;
	} else {
		key_state &= ~(key + 1);
	}
}

