
#include	"delay.h"

void  delay_ms(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

void delay_5us()		//@11.0592MHz
{
	unsigned char i;
	i = 10;
	while (--i);
}

