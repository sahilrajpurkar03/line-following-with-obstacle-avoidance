/*
 * uart.c
 *
 * Created: 29-06-2018 14:19:32
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#define BAUD 9600									//serial com;
#define BAUDRATE  ((F_CPU/(BAUD*16UL)-1))

void serialstart_3();

int value=0;

int main(void)
{
	serialstart_3();
    while (1) 
    {
		value=UDR0;
		
    }
}

void serialstart_3()
{
	UBRR0H=BAUDRATE>>8;
	UBRR0L=BAUDRATE;
	UCSR0B=0b10011000;//enable RXEN TXEN
	UCSR0C=0b00000110;// UCSZ1 UCSZ0
}

