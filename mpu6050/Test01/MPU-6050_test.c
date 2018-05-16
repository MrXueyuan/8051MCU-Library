//  MCU:STC15W408AS
//  晶振：11.0592M
// GY-52 MPU3050 IIC测试程序

/*头文件*/
#include "STC15.h"      //STC15W系列单片机头文件
#include <math.h>       //数学计算头文件
#include <stdio.h>      //8051标准串口输入输出头文件
#include <INTRINS.H>    //_nop_空函数头文件

/*简化数据类型名*/
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

/*IIC引脚定义*/
sbit    SCL=P1^0;       //IIC时钟
sbit    SDA=P1^1;       //IIC数据

// 定义MPU6050内部地址
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取

//定义类型及变量
uchar dis[6];                   //显示数字(-511至512)的字符数组
int	dis_data;                   //变量

//IIC操作预定义
void  I2C_Start();
void  I2C_Stop();
void  I2C_SendACK(bit ack);
bit   I2C_RecvACK();
void  I2C_SendByte(uchar dat);
uchar I2C_RecvByte();
void  I2C_ReadPage();
void  I2C_WritePage();
void  display_ACCEL_x();
void  display_ACCEL_y();
void  display_ACCEL_z();
uchar Single_ReadI2C(uchar REG_Address);						//读取I2C数据
void  Single_WriteI2C(uchar REG_Address,uchar REG_data);	//向I2C写入数据
void  delay(unsigned int k);

//串口
void init_uart()//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xDC;		//设定定时初值
	TH1 = 0xDC;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}

//延时函数
void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 11;
	while (--i);
}

void delay(unsigned int k)	
{						
	unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
		for(j=0;j<121;j++);
	}						
}

//初始化MPU6050
void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}


//I2C起始信号
void I2C_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}
//I2C停止信号
void I2C_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
void I2C_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}
//I2C接收应答信号
bit I2C_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
    return CY;
}
//向I2C总线发送一个字节数据
void I2C_SendByte(uchar dat)
{
    uchar i;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    I2C_RecvACK();
}
//从I2C总线接收一个字节数据
uchar I2C_RecvByte()
{
    uchar i;
    uchar dat = 0;
    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}
//向I2C设备写入一个字节数据
void Single_WriteI2C(uchar REG_Address,uchar REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}
//从I2C设备读取一个字节数据
uchar Single_ReadI2C(uchar REG_Address)
{
	uchar REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}
//IIC数据合成
int GetData(uchar REG_Address)
{
	uchar H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //合成数据
}

//数据推送至串口
void  SeriPushSend(uchar send_data)
{
    SBUF=send_data;  
	while(!TI);
    TI=0;	  
}

//整数转换字符串
void Transformation(uchar *s,int temp_data)
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s=' ';

	*++s =temp_data/10000+0x30;
	temp_data=temp_data%10000;     //取余运算

	*++s =temp_data/1000+0x30;
	temp_data=temp_data%1000;     //取余运算

	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //取余运算
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //取余运算
	*++s =temp_data+0x30; 	
}

//输出数据
void Display10BitData(int value,uchar x,uchar y)
{   uchar i;
	Transformation(dis, value);			//转换数据显示
	for(i=0;i<6;i++)
	{
        SeriPushSend(dis[i]);
    }
}

int main (void) 
{
	delay(500);		//上电延时		
	init_uart();    //初始化串口
	InitMPU6050();	//初始化MPU6050
	delay(150);     //延时
	while(1)
	{
	
		SeriPushSend('A');    
        SeriPushSend(0x20);SeriPushSend('X'); SeriPushSend(':');   
        Display10BitData(GetData(ACCEL_XOUT_H));    //显示X轴加速度  
        SeriPushSend(0x20);SeriPushSend('Y'); SeriPushSend(':');  
        Display10BitData(GetData(ACCEL_YOUT_H));    //显示Y轴加速度  
        SeriPushSend(0x20);SeriPushSend('Z'); SeriPushSend(':');  
        Display10BitData(GetData(ACCEL_ZOUT_H));    //显示Z轴加速度  
        SeriPushSend(0x20);   
        SeriPushSend('G');    
        SeriPushSend(0x20);SeriPushSend('X'); SeriPushSend(':');  
        Display10BitData(GetData(GYRO_XOUT_H));     //显示X轴角速度  
        SeriPushSend(0x20);SeriPushSend('Y'); SeriPushSend(':');  
        Display10BitData(GetData(GYRO_YOUT_H));     //显示Y轴角速度  
        SeriPushSend(0x20);SeriPushSend('Z'); SeriPushSend(':');  
        Display10BitData(GetData(GYRO_ZOUT_H));     //显示Z轴角速度          
        SeriPushSend(0x0d);   
        SeriPushSend(0x0a);//换行，回车  
        delay(2000); 
	}
}