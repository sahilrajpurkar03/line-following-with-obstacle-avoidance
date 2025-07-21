/*
 * Buzzer.c
 *
 * Created: 19-06-2018 10:49:07
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

void buzzer_init();
void buzzer_on();
void buzzer_off();

int main(void)
{
	buzzer_init();
    while (1) 
    {
		buzzer_on();
		_delay_ms(1000);
		buzzer_off();
		_delay_ms(1000);
    }
}

void buzzer_init()
{
	DDRC|=0x08;
}

void buzzer_on()
{
	PORTC|=0x08;
}

void buzzer_off()
{
	PORTC&=0xF7;
}
	
	
}