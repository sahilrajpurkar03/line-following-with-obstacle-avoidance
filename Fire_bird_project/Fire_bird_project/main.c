/*
 * Fire_bird_project.c
 *
 * Created: 21-06-2018 12:17:24
 * Author : Sahil
 * Description: This code controls a Fire Bird robot to perform basic autonomous movement using sensors and PWM.
 */

#define F_CPU 14745600UL

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

// Global variables for sensor values and state
int s_mode = 0, a, b, c, d;

// Function declarations
void port_init();
void fwd();
void fwd_left();
void fwd_right();
void stop();
void timer_init();
unsigned char ADC_Conversion(unsigned char ch);
void adc_init();
void linefollow();
void readprint();
void process();

int main(void)
{
	port_init();      // Initialize motor and sensor ports
	adc_init();       // Initialize ADC for sensor readings
	init_ports();     // Initialize LCD ports
	lcd_init();       // Initialize LCD
	timer_init();     // Initialize PWM timer for motor control

	while (1) 
	{ 
		readprint();   // Read sensors and display values on LCD
		process();     // Process logic and control movement
	}
}

/*
 * Function: port_init
 * -------------------
 * Initializes ports:
 * - PORTF: Input (sensors)
 * - PORTA: Output (motor control)
 * - PORTL: Output (PWM)
 */
void port_init()
{
	DDRF  = 0x00;  // PORTF as input (sensors)
	PORTF = 0x00;

	DDRA  |= 0x0F; // Lower 4 bits of PORTA as output (motors)
	PORTA &= 0xF0;

	DDRL  |= 0x18; // Bits 3 and 4 of PORTL for PWM output (OCR5A, OCR5B)
}

/*
 * Function: process
 * -----------------
 * FSM logic based on sensor input 'd'
 */
void process()
{
	switch (s_mode)
	{
		case 0: 
			if (d < 250)
			{
				s_mode = 1;
			}
			else if (d >= 250 && d < 512)
			{
				s_mode = 2;
			}
			break;

		case 1:
			linefollow(); // Line following mode
			break;

		case 2:
			fwd_left();      // Turn left for 1 second
			_delay_ms(1000);
			if (a)
			{
				linefollow(); // Continue line following if left sensor detects line
			}
			break;
	}
}

/*
 * Function: fwd
 * -------------
 * Move robot forward with defined PWM speeds
 */
void fwd()
{
	PORTA &= 0xF0;
	PORTA |= 0x06;     // Forward motion
	OCR5A = 255;       // Left wheel speed
	OCR5B = 230;       // Right wheel speed
}

/*
 * Function: fwd_left
 * ------------------
 * Turn robot slightly left
 */
void fwd_left()
{
	PORTA &= 0xF0;
	PORTA |= 0x06;
	OCR5A = 150;
	OCR5B = 220;
}

/*
 * Function: fwd_right
 * -------------------
 * Turn robot slightly right
 */
void fwd_right()
{
	PORTA &= 0xF0;
	PORTA |= 0x06;
	OCR5A = 235;
	OCR5B = 150;
}

/*
 * Function: stop
 * --------------
 * Stop all motor movement
 */
void stop()
{
	PORTA &= 0xF0;
	PORTA |= 0x00;
}

/*
 * Function: timer_init
 * --------------------
 * Initializes Timer5 for PWM generation
 */
void timer_init()
{
	TCCR5A |= (1 << WGM50) | (1 << COM5A1) | (1 << COM5B1); // Fast PWM, non-inverting
	TCCR5B |= (1 << WGM52) | (1 << CS50) | (1 << CS52);      // Prescaler = 1024
}

/*
 * Function: adc_init
 * ------------------
 * Initialize ADC for analog sensor reading
 */
void adc_init()
{
	ADMUX  |= (1 << REFS0); // Vref = AVCC
	ADCSRA |= (1 << ADEN)   // Enable ADC
	        | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler = 128
}

/*
 * Function: ADC_Conversion
 * ------------------------
 * Read analog value from ADC channel
 */
unsigned char ADC_Conversion(unsigned char ch)
{
	unsigned char value;

	if (ch > 7)
	{
		ADCSRB = 0x08; // Enable MUX5 for channels > 7
	}

	ch &= 0x07;
	ADMUX = 0x20 | ch;          // Select channel with left adjust
	ADCSRA |= 0x40;             // Start conversion

	while ((ADCSRA & 0x10) == 0); // Wait for conversion to complete
	value = ADCH;
	ADCSRA |= 0x10;             // Clear ADC interrupt flag
	ADCSRB = 0x00;              // Clear MUX5

	return value;
}

/*
 * Function: linefollow
 * --------------------
 * Basic line following logic based on 3 sensors
 */
void linefollow()
{
	if (b < 15 && a >= 25 && c >= 25)
	{
		fwd(); // Move forward
	}
	else if (c < 15)
	{
		fwd_left(); // Slight left
	}
	else if (a < 15)
	{
		fwd_right(); // Slight right
	}
	else if (a > 25 && b > 25 && c > 25)
	{
		stop(); // Stop if no line
	}
}

/*
 * Function: readprint
 * -------------------
 * Read sensor values and display on LCD
 */
void readprint()
{
	a = ADC_Conversion(1); // Left sensor
	b = ADC_Conversion(2); // Center sensor
	c = ADC_Conversion(3); // Right sensor
	d = ADC_Conversion(5); // Front proximity sensor

	lcd_print(1, 1, c, 3); // Display right sensor
	lcd_print(1, 5, b, 3); // Display center sensor
	lcd_print(1, 9, a, 3); // Display left sensor
	lcd_print(2, 1, d, 3); // Display proximity
}
