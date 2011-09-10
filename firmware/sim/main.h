#include <stdint.h>

#define LED_WIDTH	4
#define LED_HEIGHT	5

#define ATTRIBUTES	__attribute__((constructor));

void setLedXY(uint8_t x, uint8_t y, uint8_t brightness);
void registerAnimation(uint8_t (*fp)(void), uint16_t t, uint16_t duration);

