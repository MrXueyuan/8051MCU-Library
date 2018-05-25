#include "LCD1602.h"
#include "delay.h"
#include "config.h"
#include "Keyboard.h"

#include "STC12.h"

char key_namber = 0;
unsigned char data password [] = {0,0,0,0,0,0};
unsigned char data enter_psw [] ;


void LCD_Refresh () {
	
}

int main (void) {
	delay_ms (5);
	
	Initialize_LCD();
	ClearLine(1);
	ClearLine(2);
	PutString(1,1,"Fuck Bug!");
	
	while (1) {
		unsigned char links_L = 0;
		key_namber = read_key ();
		if (key_namber != -1) {
			if (key_namber ==3) {
				PutString(2,links_L+1,'*');
				links_L++;
				if (links_L>=4) {
					links_L = 0;
				}
			}
		}
	}
}
