#include "LCD2004.h"

void LCD_NOPDelay()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 107;
	while (--i);
}

//忙检测
void check_BF(void)
{
	DATA=0xff;					//赋数据口初始值 
	E=0;						//读状态前三控制线的状态 
	RS=0;
	RW=1;
	E=1;						//拉高，读状态 
	while((DATA&0x80)==0x80);	//判断是否繁忙，等待空闲 
	E=0;						//读状态后三控制线的状态 
	RS=1;
	RW=0;
}

//写指令函数
void write_cmd(unsigned char com)
{
	check_BF();
	E=0;			//写命令前三控制线的状态 
	RS=0;
	RW=0;
	DATA=com;
	LCD_NOPDelay();
	LCD_NOPDelay();
	E=1;
	LCD_NOPDelay();
	LCD_NOPDelay();
	E=0;			//写命令后三控制线的状态 
	RS=1; 
	RW=1;
}

//写数据函数
void write_data(unsigned char chardata)
{
	check_BF();
	E=0;			//写数据前三控制线的状态
	RS=1;
	RW=0;
	DATA=chardata;
	LCD_NOPDelay();
	LCD_NOPDelay();
	E=1;
	LCD_NOPDelay();
	LCD_NOPDelay();
	E=0;			//写数据后三控制线的状态 
	RS=0; 
	RW=1;
}

//液晶初始化函数
void Init_LCD(void)
{
	write_cmd(0x38);	//设置8位格式，2行，5x7
	write_cmd(0x08);	//调整屏幕关闭
	write_cmd(0x01);	//设定输入方式，增量不移位
	write_cmd(0x06);	//整体显示，关光标，不闪烁
	write_cmd(0x0c);	//清除屏幕显示                        
}

//定义显示位置
void set_xy(unsigned char x,unsigned char y)
{
	unsigned char add;
	
	if(x==0)
		add=0x80+y;
	else if(x==1)
		add=0xc0+y;
	else if(x==2)
		add=0x94+y;
	else if(x==3)
		add=0xd4+y;
	
	write_cmd(add);
}

//显示函数
void display(unsigned char x,unsigned char y,unsigned char *s)
{
	set_xy(x,y);
	while(*s)
	{
		DATA=*s;
		write_data(*s);
		s++;
	}
}

