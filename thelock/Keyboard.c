#include 	"Keyboard.h"
#include	"config.h"
#include 	"delay.h"
char read_key (void) {
	unsigned char i,j;
	unsigned char key_val[]={0xFA,0xF9,0x7B,0xBB,0xF6,0xF5,0x77,0xB7,0xEE,0xED,0x6F,0xAF,0xDE,0xDD,0x5F,0x9F};
	unsigned char key_scan[]={0xFB,0xF7,0xEF,0xDF};
	P3=0xC3;
	if(P3!=0xC3)
	{	
		delay_ms(2);
		if(P3!=0xC3)
		{	
			for(i=0;i<4;i++)
			{
				P3=key_scan[i];
				for(j=0;j<16;j++)
				if(key_val[j]==P3)
					return j;
				else return -1; 
			}  	
		}
		else return -1;
	}
	else return -1;
}