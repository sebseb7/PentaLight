#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "main.h"
#include "leds.h"

uint16_t onstate = 0;
uint8_t  nextevent = 0;
uint8_t leds[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t volatile leds_buf[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t volatile timeout = 0;

uint8_t adc_timeout = 0;
uint8_t tick_timeout = 0;
uint8_t volatile tick_interval = 18;
uint8_t volatile call_tick = 0;

uint16_t volatile off_period = 500;

typedef void (*AppPtr_t)(void) __attribute__ ((noreturn));

//0,1,2,5,12,26,58,127
//0,1,2,4,8,16,32,64
//0,1,1,1,2,3,3,5,7,9,13,17,24,33,46,63

//122hz
ISR (TIMER0_OVF_vect)
{
	if(timeout > 0) timeout--;
	adc_timeout++;
	tick_timeout++;
	
	// ~ every 2 seconds
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
	
/*	
	
	if(ADCL & 1)
	{
		setLed(3,3);
	}
	else
	{
		setLed(3,0);
	}
	if(ADCL & 2)
	{
		setLed(7,3);
	}
	else
	{
		setLed(7,0);
	}
	if(ADCL & 4)
	{
		setLed(11,3);
	}
	else
	{
		setLed(11,0);
	}
	if(ADCL & 8)
	{
		setLed(10,3);
	}
	else
	{
		setLed(10,0);
	}
	if(ADCL & 16)
	{
		setLed(9,3);
	}
	else
	{
		setLed(9,0);
	}
	if(ADCL & 32)
	{
		setLed(8,3);
	}
	else
	{
		setLed(8,0);
	}
	if(ADCL & 64)
	{
		setLed(12,3);
	}
	else
	{
		setLed(12,0);
	}
	if(ADCL & 128)
	{
		setLed(16,3);
	}
	else
	{
		setLed(16,0);
	}


	if(ADCH & 1)
	{
		setLed(18,3);
	}
	else
	{
		setLed(18,0);
	}

	if(ADCH & 2)
	{
		setLed(19,3);
	}
	else
	{
		setLed(19,0);
	}
*/
}


//32khz
ISR (TIMER1_OVF_vect)
{
//	LED_03_ON;

	if(onstate == 0)
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
		
		onstate = 1;
		return;
		
	}
	else if((onstate == 1)||(onstate == 10)||(onstate == 70))
	{
		if(onstate == leds[0]) LED_00_OFF;
		if(onstate == leds[1]) LED_01_OFF;
		if(onstate == leds[2]) LED_02_OFF;
		if(onstate == leds[3]) LED_03_OFF;
		if(onstate == leds[4]) LED_04_OFF;
		if(onstate == leds[5]) LED_05_OFF;
		if(onstate == leds[6]) LED_06_OFF;
		if(onstate == leds[7]) LED_07_OFF;
		if(onstate == leds[8]) LED_08_OFF;
		if(onstate == leds[9]) LED_09_OFF;
		if(onstate == leds[10]) LED_10_OFF;
		if(onstate == leds[11]) LED_11_OFF;
		if(onstate == leds[12]) LED_12_OFF;
		if(onstate == leds[13]) LED_13_OFF;
		if(onstate == leds[14]) LED_14_OFF;
		if(onstate == leds[15]) LED_15_OFF;
		if(onstate == leds[16]) LED_16_OFF;
		if(onstate == leds[17]) LED_17_OFF;
		if(onstate == leds[18]) LED_18_OFF;
		if(onstate == leds[19]) LED_19_OFF;

		onstate++;
		return;

	}


	onstate++;
	

//bad batteries : 64: 500hz

//good batteries 1ff: 62hz

	if(onstate > off_period)
	{
		onstate = 0;

		for(uint8_t i = 0;i<20;i++)
		{
			leds[i]=leds_buf[i];
		}

	}
	

//	LED_03_OFF;
}
ISR (PCINT2_vect)
{
	// for now we jump to the bootloader when any button is pressed
	GPIOR2=255;
	PRR = 0;
	AppPtr_t AppStartPtr = (AppPtr_t)0x1800;
	AppStartPtr();
}

uint8_t (*tick_fp[10])(void);
uint16_t interval_count[10];
uint16_t interval_duration[10];


uint8_t animations = 0;

void registerAnimation(uint8_t (*fp)(void),uint16_t tickInterval, uint16_t intervals)
{
	tick_fp[animations] = fp;
	interval_count[animations]=intervals;
	interval_duration[animations]=tickInterval;
	
	animations++;
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

	//disale input pin C5 (adc pin)
	DIDR0 |= (1<<ADC5D);
	
	
	//enable pullups for buttons
	PORTD |= (1<<PORTD0)|(1<<PORTD1);
	
	//enable pcint
	PCMSK2 |= (1<<PCINT16)|(1<<PCINT17);
	PCICR = (1<<PCIE2);


	//set timer1 to FastPWM Mode & prescaler 1
	TCCR1A |= (1<<WGM10)|(1<<WGM11);
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS10);
	OCR1A = 250;//TIMER_FREQ; 32khz
	//enable interrupt
	TIMSK1 |= (1<<TOIE1);

	//set timer0 to normal Mode & prescaler 256 == 122Hz
	TCCR0B |= (1<<CS02);
	//enable interrupt
	TIMSK0 |= (1<<TOIE0);

	// select adc5 and interval 1.1v reference
	// adc5 pin is connecgted to vcc with a 1/3 divider
	ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<MUX2)|(1<<MUX0);
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1);
	
	

//	SMCR = (1<<SE);


	sei();

	while(1)
	{
		for(uint8_t i =0;i < animations;i++)
		{
			if(interval_count[i] != 0)
			{
				for(uint16_t j=0;j < interval_count[i];)
				{ 
					if(call_tick == 1)
					{
						call_tick =0;
						(*tick_fp[i])();
						j++;
					}
					//		volatile asm("sleep");
				}
			}
			else
			{
				uint8_t i  = 0;
				while(i == 0)
				{
					if(call_tick == 1)
					{
						call_tick =0;
						i = (*tick_fp[i])();
					}
					//		volatile asm("sleep");
				}
			}
		}
	}
}


void setLed(uint8_t led_nr,uint8_t brightness)
{
	if((led_nr < 20)&&(brightness < 4))
	{
		if(brightness == 0)	leds_buf[led_nr] = 0;
		if(brightness == 1)	leds_buf[led_nr] = 1;
		if(brightness == 2)	leds_buf[led_nr] = 10;
		if(brightness == 3)	leds_buf[led_nr] = 70;
	}
}

void setLedXY(uint8_t x,uint8_t y, uint8_t brightness)
{
	x = 3-x;
	if((x < 4)&&(y<5)&&(brightness < 4))
	{
		if(brightness == 0)	leds_buf[y*4+x] = 0;
		if(brightness == 1)	leds_buf[y*4+x] = 1;
		if(brightness == 2)	leds_buf[y*4+x] = 10;
		if(brightness == 3)	leds_buf[y*4+x] = 70;
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
