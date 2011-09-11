#include <stdint.h>

#define LED_WIDTH	4
#define LED_HEIGHT	5

#define SIMULATOR

#define ATTRIBUTES	__attribute__((constructor));

typedef uint8_t (*tick_fun)(void);

void setLedXY(uint8_t x, uint8_t y, uint8_t brightness);
void registerAnimation(tick_fun fp, uint16_t t, uint16_t duration);

