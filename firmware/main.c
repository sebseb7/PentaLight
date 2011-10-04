#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"
#include "leds.h"

uint16_t pwmState = 0;
uint8_t leds[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t volatile leds_buf[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t volatile current_adc_value;


uint8_t adc_timeout = 0;
uint8_t tick_timeout = 0;
uint8_t volatile tick_interval = 16;
uint8_t volatile call_tick = 0;
uint8_t volatile key_event = 0;

uint16_t volatile off_period = 500;

typedef void (*AppPtr_t)(void) __attribute__ ((noreturn));

//122hz
ISR (TIMER0_OVF_vect)
{
	adc_timeout++;
	tick_timeout++;
	
	// 0.47Hz == ~ every 2 seconds
	if(adc_timeout == 0xff)
	{
		adc_timeout = 0;
		ADCSRA |= (1<<ADSC);
	}

	if(tick_timeout == tick_interval)
	{
		tick_timeout = 0;
		call_tick = 1;
	}
}

ISR(ADC_vect)
{
	uint16_t new_off_period = 500;

	current_adc_value = ADC;

	//improove randomness
	srand(ADC);

	if(ADC < 978)
	{
		uint16_t tmp = 978-ADC;
		new_off_period -= tmp;
		if(new_off_period < 64)
		{
			new_off_period=64;
		}
	}
	off_period=new_off_period;
	
}


//32khz
ISR (TIMER1_OVF_vect)
{
	if(pwmState == 0)
	{
		if(leds[0] != 0) LED_00_ON;
		if(leds[1] != 0) LED_01_ON;
		if(leds[2] != 0) LED_02_ON;
		if(leds[3] != 0) LED_03_ON;
		if(leds[4] != 0) LED_04_ON;
		if(leds[5] != 0) LED_05_ON;
		if(leds[6] != 0) LED_06_ON;
		if(leds[7] != 0) LED_07_ON;
		if(leds[8] != 0) LED_08_ON;
		if(leds[9] != 0) LED_09_ON;
		if(leds[10] != 0) LED_10_ON;
		if(leds[11] != 0) LED_11_ON;
		if(leds[12] != 0) LED_12_ON;
		if(leds[13] != 0) LED_13_ON;
		if(leds[14] != 0) LED_14_ON;
		if(leds[15] != 0) LED_15_ON;
		if(leds[16] != 0) LED_16_ON;
		if(leds[17] != 0) LED_17_ON;
		if(leds[18] != 0) LED_18_ON;
		if(leds[19] != 0) LED_19_ON;
		
		pwmState = 1;
		return;
		
	}
	else if((pwmState == 1)||(pwmState == 2)||(pwmState == 4)||(pwmState == 8)||(pwmState == 20)||(pwmState == 40)||(pwmState == 70))
	{
		if(pwmState == leds[0]) LED_00_OFF;
		if(pwmState == leds[1]) LED_01_OFF;
		if(pwmState == leds[2]) LED_02_OFF;
		if(pwmState == leds[3]) LED_03_OFF;
		if(pwmState == leds[4]) LED_04_OFF;
		if(pwmState == leds[5]) LED_05_OFF;
		if(pwmState == leds[6]) LED_06_OFF;
		if(pwmState == leds[7]) LED_07_OFF;
		if(pwmState == leds[8]) LED_08_OFF;
		if(pwmState == leds[9]) LED_09_OFF;
		if(pwmState == leds[10]) LED_10_OFF;
		if(pwmState == leds[11]) LED_11_OFF;
		if(pwmState == leds[12]) LED_12_OFF;
		if(pwmState == leds[13]) LED_13_OFF;
		if(pwmState == leds[14]) LED_14_OFF;
		if(pwmState == leds[15]) LED_15_OFF;
		if(pwmState == leds[16]) LED_16_OFF;
		if(pwmState == leds[17]) LED_17_OFF;
		if(pwmState == leds[18]) LED_18_OFF;
		if(pwmState == leds[19]) LED_19_OFF;

		pwmState++;
		return;

	}


	pwmState++;
	

//bad batteries : 64: 500hz

//good batteries 1ff: 62hz

	if(pwmState > off_period)
	{
		pwmState = 0;

		for(uint8_t i = 0;i<20;i++)
		{
			leds[i]=leds_buf[i];
		}

	}
	

}
ISR (PCINT2_vect)
{
	key_event = 1;

/*	GPIOR2=255;
	PRR = 0;
	AppPtr_t AppStartPtr = (AppPtr_t)0x1800;
	AppStartPtr();*/
}

#define MAX_NUMBER_OF_ANIMATIONS 10
#define MAX_NUMBER_OF_APPS 2

uint8_t (*appTick_fp[MAX_NUMBER_OF_APPS])(void);
void (*appKey_fp[MAX_NUMBER_OF_APPS])(key_type,event_type);
uint16_t appInterval_duration[MAX_NUMBER_OF_APPS];

uint8_t (*aniTick_fp[MAX_NUMBER_OF_ANIMATIONS])(void);
uint16_t aniInterval_count[MAX_NUMBER_OF_ANIMATIONS];
uint16_t aniInterval_duration[MAX_NUMBER_OF_ANIMATIONS];


uint8_t animations = 0;
uint8_t apps = 0;

void registerAnimation(uint8_t (*fp)(void),uint16_t tickInterval, uint16_t intervals)
{
	if(animations < MAX_NUMBER_OF_ANIMATIONS)
	{
		aniTick_fp[animations] = fp;
		aniInterval_count[animations]=intervals;
		aniInterval_duration[animations]=tickInterval;
	
		animations++;
	}
}

void registerApp(uint8_t (*fp)(void),void (*key_fp)(key_type,event_type), uint16_t tickInterval)
{
	if(apps < MAX_NUMBER_OF_APPS)
	{
		appTick_fp[apps] = fp;
		appKey_fp[apps] = key_fp;
		appInterval_duration[apps]=tickInterval;
	
		apps++;
	}
}


int main (void)
{

	//enable LED channels as output
	PORTB |= (1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5)|(1<<PORTB6)|(1<<PORTB7);
	PORTC |= (1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC6);
	PORTD |= (1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	
	DDRB |= (1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5)|(1<<PORTB6)|(1<<PORTB7);
	DDRC |= (1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC6);//C6 is reset, C5 is ADC, c7 is not available
	DDRD |= (1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);// d0 is RX, d1 is TX


	//reduce power
	PRR |= (1<<PRTWI)|(1<<PRTIM2)|(1<<PRSPI)|(1<<PRUSART0);

	//disable input pin C5 (adc pin)
	DIDR0 |= (1<<ADC5D);
	
	
	//enable pullups for buttons
	PORTD |= (1<<PORTD0)|(1<<PORTD1);
	
	//enable pcint for buttons
	PCMSK2 |= (1<<PCINT16)|(1<<PCINT17);
	PCICR = (1<<PCIE2);


	// Timer 1 ist used for application timing and ADC timing
	
	//set timer1 to FastPWM Mode & prescaler 1
	TCCR1A |= (1<<WGM10)|(1<<WGM11);
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS10);
	OCR1A = 250;//TIMER_FREQ; 32khz
	//enable interrupt
	TIMSK1 |= (1<<TOIE1);


	// Timer 0 ist used for LED PWM
	
	//set timer0 to normal Mode & prescaler 256 == 122Hz
	TCCR0B |= (1<<CS02);
	//enable interrupt
	TIMSK0 |= (1<<TOIE0);

	// select adc5 and interval 1.1v reference
	// adc5 pin is connecgted to vcc with a 1/3 divider
	ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<MUX2)|(1<<MUX0);
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1);
	
	

	SMCR = (1<<SE);


	sei();

	uint8_t activeApp = 0; //0 == animation loop ; 1 == app1 ; 2 == app2
	uint8_t current_animation = 0;
	uint16_t current_ani_tick = 0;
	uint16_t current_app_tick = 0;
	uint8_t left_key = 0;
	uint8_t right_key = 0;
	
	tick_interval = aniInterval_duration[current_animation];


	while(1)
	{
		if(key_event == 1)
		{
			
			//as soon a we get the same reading 30 times in a row, we consider the pin debounced
			uint8_t current=0;
			uint8_t same=0;
			while(1)
			{
				if((PIND&3)==current)
				{
					same++;
				}
				else
				{
					same=0;
					current = (PIND&3);
				}
				
				if(same==30)
				{
					break;
				}
			}
			
			
			
			if(activeApp==0)
			{
				if((apps>0)&&(current==1))
				{
					current_app_tick=0;
					activeApp = 1;
					tick_interval = appInterval_duration[0];
					clearAllLeds();
				}
				if((apps>1)&&(current==2))
				{
					current_app_tick=0;
					activeApp = 2;
					tick_interval = appInterval_duration[1];
					clearAllLeds();
				}
			}
			else
			{
				key_type key;
				event_type event;
				if(((current&1)==1)&&(left_key == 0))
				{
					left_key = 1;
					key = KEY_B;
					event = UP;
					(*appKey_fp[activeApp-1])(key,event);
				}else if(((current&1)==0)&&(left_key == 1))
				{
					left_key = 0;
					key = KEY_B;
					event = DOWN;
					(*appKey_fp[activeApp-1])(key,event);
				}


				if(((current&2)==2)&&(right_key == 0))
				{
					right_key = 1;
					key = KEY_A;
					event = UP;
					(*appKey_fp[activeApp-1])(key,event);
				}else if(((current&2)==0)&&(right_key == 1))
				{
					right_key = 0;
					key = KEY_A;
					event = DOWN;
					(*appKey_fp[activeApp-1])(key,event);
				}
				
			}
			key_event=0;
		}

		if(call_tick==1)
		{
			if(activeApp == 0)
			{
				if(current_ani_tick >= aniInterval_count[current_animation])
				{
					current_ani_tick=0;
					current_animation++;
					if(current_animation == animations)
					{
						current_animation = 0;
					}
					tick_interval = aniInterval_duration[current_animation];
				}
			
				(*aniTick_fp[current_animation])();

				current_ani_tick++;
			}
			else
			{
				uint8_t retval = (*appTick_fp[activeApp-1])();
				if(retval == 1)
				{
					activeApp = 0;
					tick_interval = aniInterval_duration[current_animation];
				}
			}
			call_tick=0;
		}
		
		asm volatile("sleep");
	}
}


void setLed(uint8_t led_nr,uint8_t brightness)
{
	if((led_nr < 20)&&(brightness < 8))
	{
		if(brightness == 0)	leds_buf[led_nr] = 0;
		if(brightness == 1)	leds_buf[led_nr] = 1;
		if(brightness == 2)	leds_buf[led_nr] = 2;
		if(brightness == 3)	leds_buf[led_nr] = 4;
		if(brightness == 4)	leds_buf[led_nr] = 8;
		if(brightness == 5)	leds_buf[led_nr] = 20;
		if(brightness == 6)	leds_buf[led_nr] = 40;
		if(brightness == 7)	leds_buf[led_nr] = 70;
	}
}

void clearAllLeds(void)
{
	for(uint8_t i = 0;i<20;i++)
	{
		leds_buf[i]=0;
	}
}


void setLedXY(uint8_t x,uint8_t y, uint8_t brightness)
{
	if((x < 4)&&(y<5)&&(brightness < 8))
	{
		if(brightness == 0)	leds_buf[y*4+x] = 0;
		if(brightness == 1)	leds_buf[y*4+x] = 1;
		if(brightness == 2)	leds_buf[y*4+x] = 2;
		if(brightness == 3)	leds_buf[y*4+x] = 4;
		if(brightness == 4)	leds_buf[y*4+x] = 8;
		if(brightness == 5)	leds_buf[y*4+x] = 20;
		if(brightness == 6)	leds_buf[y*4+x] = 40;
		if(brightness == 7)	leds_buf[y*4+x] = 70;
	}
}

void scrollLeft()
{
	for(uint8_t i = 0;i < 5;i++)
	{
		leds_buf[19-(i*4)] = leds_buf[18-(i*4)];
		leds_buf[18-(i*4)] = leds_buf[17-(i*4)];
		leds_buf[17-(i*4)] = leds_buf[16-(i*4)];
		leds_buf[16-(i*4)] = 0;
	}


}
