#include "stc12.h"
#define FOSC  11059200L
#define BAUD 115200
//sfr AUXR  =0x8E;
sfr TH2   =0xD6;
sfr TL2   =0xD7;
bit busy=0;
xdata char menu[]={"\r\n------main menu---------------"
	                 "\r\n     input 1:  ��ת io P1^0"
                   "\r\n     input 2:  ��ת io P1^1"
	                 "\r\n     other  :  ��������"
	                 "\r\n------end menu----------------"
									 "\r\n"
            };
	
void SendData(unsigned char dat)
{
	while(busy);
	SBUF=dat;
	busy=1;
}
void SendString(char *s)
{
	while(*s!='\0')
		SendData(*s++);
}
void uart1() interrupt 4
{
	if(RI)
		RI=0;
	if(TI)
		TI=0;
	  busy=0;
}
		
void main()
{
	unsigned char c;
	P10=0;
	P11=0;
	SCON=0x50;
	AUXR=0x14;
	AUXR|=0x01;
	TL2=(65536-((FOSC/4)/BAUD));
	TH2=(65536-((FOSC/4)/BAUD))>>8;
	ES=1;
	EA=1;
	 SendString(&menu);
	 while(1){
		 if(RI==1)
		 {
			 c=SBUF;
			 if(c==0x31) {
				 P10=!P10;
				 SendString ("\r\n������ִ��: IO��-P1^0��ת.\r\n");
			 }
			 else if(c==0x32) {
				 P11=!P11;
				 SendString ("\r\n������ִ��: IO��-P1^1��ת.\r\n");
			 }
			 else 
			 {
				   SendString("\r\n ---Res---");
					 SendString(&menu);
			 }
		 }    
	 }
}
	