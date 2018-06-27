#include "MCU_Doc.h"

#define LED P20
#define DP  P0
#define DW1 P10
#define DW2 P11
#define DW3 P12
#define DW4 P13

u8  count   = 0;
u8  second  = 0;
u8  minute  = 0;
u8  hour    = 0;

//延时函数
void Delay1ms(uchar DTime) 
{
    while (--DTime) {
        uchar i, j;
        i = 2;
        j = 239;
        do {
            while (--j);
        }
        while (--i);
    }
}

//中断程序
void InitFunction (void) interrupt 1 using 1 
{
    
    TL0 = 0xB0;		//设置定时初值
    TH0 = 0x3C;		//设置定时初值
    count++;
    if (count>=20) {

        count = 0;
        second++;
        LED = ~LED;

        if (second>=60) {

            second = 0;
            minute++;

            if (minute>=60) {

                minute = 0;
                hour++;

                if (hour>=24)
                    hour = 0;
            }
        }
    }
    if (count == 8)
        LED = 1;
}

void display_show (void) 
{
    //小时-十位
    DW1 = 0;
    if ((hour/10)!=0)
        DP  = a_displays [hour/10];
    else if (count>=10)
        DP  = a_displays [16];
    else
        DP  = a_displays [17];
    Delay1ms (1);
    DW1 = 1;
    DP  = a_displays [16];
    //小时-个位
    DW2 = 0;
    if (hour%10==0)
        if ((hour/10)!=0)
            DP  = a_displays [0];
        else if (count>=10)
            DP  = a_displays [16];
            else
                DP  = a_displays [17];
    else
        DP  = a_displays [hour%10];
    Delay1ms (1);
    DP  = 0x7F;
    Delay1ms (1);
    DW2 = 1;
    DP  = a_displays [16];
    //分钟-十位
    DW3 = 0;
    if ((minute/10)!=0)
        DP  = a_displays [minute/10];
    else if (count>=10)
        DP  = a_displays [16];
    else
        DP  = a_displays [17];
    Delay1ms (1);
    DW3 = 1;
    DP  = a_displays [16];
    //分钟-个位
    DW4 = 0;
    if (minute%10==0)0
        if ((minute/10)!=0)
            DP  = a_displays [0];
        else if (count>=10)
            DP  = a_displays [16];
            else
                DP  = a_displays [17];
    else
        DP  = a_displays [minute%10];
    Delay1ms (1);
    DW4 = 1;
    DP  = a_displays [16];
}

//定时器0初始化
void Timer0Init(void)   //50毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0xB0;		//设置定时初值
	TH0 = 0x3C;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

int main (void) 
{
    Delay1ms (5);       //上电延时

    EA = 1; ET0 = 1;    //打开中断
    Timer0Init ();	//定时器0初始化
    
    //端口初始化复位
    LED = 1;
    DW1 = 1;
    DW2 = 1;
    DW3 = 1;
    DW4 = 1;

    while (1) {
	    display_show ();
    }
	while (1);
}
