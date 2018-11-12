#ifndef	_LCD2004_H
#define	_LCD2004_H

#include "STC15.h"

#define	DATA P0

sbit RS	= P1^0;	//寄存器选择位（0指令1数据）
sbit RW	= P1^1;	//设置读写位（0写入1读取）
sbit E	= P2^5;	//使能为(0禁止1允许)

extern void Init_LCD(void);													//初始化LCD2004
extern void write_cmd(unsigned char com);										//写指令
extern void write_data(unsigned char chardata);										//写数据
extern void check_BF(void);													//检测忙信号
extern void set_xy(unsigned char x,unsigned char y);						//显示位置
extern void display(unsigned char x,unsigned char y,unsigned char *s);		//显示

#endif
