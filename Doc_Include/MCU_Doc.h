#ifndef _MCU_DOC_H
#define _MCU_DOC_H

#include "STC.h"
#include <math.h>
#include <stdio.h>
#include <intrins.h>

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

//共阴极数码管预定义字库
uchar code c_displays[] = {
//	 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black	 -   
	0x00,0x40
};

//共阳极数码管预定义字库
uchar code a_displays[] = {
//	 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,
//black  -
    0xFF,0xBF
};

#endif
