#include <stdint.h>

#define LED_WIDTH	4
#define LED_HEIGHT	5

void setLedXY(uint8_t x, uint8_t y, uint8_t b);

void registerAnimation(void (*)(void), uint16_t);

