#include <main.h>

#include <stdlib.h>

#define CYCLE(x,c)		((x + c) % c)
#define ARRAY_SIZE(array)	(sizeof(array) / sizeof(*array))

#define MAX_TICK		2

static void init(void) ATTRIBUTES;
static uint8_t tick(void);
static void key(key_type key, event_type event);
void draw_winner1(void);
void draw_winner2(void);

static uint8_t ticks = 0;
static uint8_t left_bar = 3;
static uint8_t right_bar = 3;
static uint8_t flash_light = 1;
static uint8_t winner = 0;
static uint8_t end = 0;

void init(void) {
	registerApp(tick, key, 22);
}

uint8_t tick() {
	int8_t y, c;
    for(y = 0; y < LED_HEIGHT; y++) {
        setLedXY(0, y,
            y <  left_bar / 7 ? 7 :
            y == left_bar / 7 ? left_bar % 7 : 0);
        setLedXY(3, y,
            y <  right_bar / 7 ? 7 :
            y == right_bar / 7 ? right_bar % 7 : 0);
    }
    if(flash_light > 0) {
        draw_flash(7);
        --flash_light;
    } else {
        draw_flash(1);
        flash_light = rand()%7;
    }
    if(winner == 1) draw_winner1();
    if(winner == 2) draw_winner2();
    if(winner > 0 && --end == 0) return 1;
	return 0;
}

void key(key_type key, event_type event) {
	if(event == DOWN && end == 0) {
		if(key == KEY_A) {
		    right_bar += flash_light > 0 ? 1 : -2;
			if (right_bar >= LED_HEIGHT * 7) {
			    winner = 1;
			}
		} else {
			left_bar += flash_light > 0 ? 1 : -2;
		    if (left_bar >= LED_HEIGHT * 7) {
		        winner = 2;
		    }
		}
		if(winner > 0) end = 10;
	}
}

void draw_flash(uint8_t b) {
    setLedXY(1, 3, b);
    setLedXY(1, 4, b);
    setLedXY(2, 3, b);
    setLedXY(2, 4, b);
}

void draw_winner1(void) {
    setLedXY(2, 1, 7);
    setLedXY(1, 2, 7);
    setLedXY(1, 0, 7);
}

void draw_winner2(void) {
    setLedXY(1, 1, 7);
    setLedXY(2, 2, 7);
    setLedXY(2, 0, 7);
}

