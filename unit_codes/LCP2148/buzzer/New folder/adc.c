#include <lpc214x.h>
#include "lcd.h"

unsigned int x=0,y=0;

int main(void)
{
	LCD_Init();
	LCD_4Bit_Mode();

	Init_Peripherals();
	PINSEL1|=(1<<24);
	PINSEL1&=(~(1<<25));				//pin 28 set to adc pin 

		AD0CR=0x01200D02;

	
	while(1)
	{
				LCD_Command(0x01);
				AD0CR|=0x01000000;
				while(!( (x=AD0GDR) & 0x800000000 ))
				y=(x<<6) & (0x3FF);
				LCD_Print(1,1,y,4);
				DelaymSec(100);
	}
}