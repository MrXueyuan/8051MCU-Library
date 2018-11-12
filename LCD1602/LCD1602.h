
#ifndef	__LCD1602_H
#define	__LCD1602_H

#include	"config.h"

void	Initialize_LCD(void);
void	Write_AC(u8 hang,u8 lie);
void	Write_DIS_Data(u8 DIS_Data);
void	ClearLine(u8 hang);
void    ClearBit(u8 hang,u8 lie);
u8		BIN_ASCII(u8 tmp);
void 	PutString(u8 hang,u8 lie,u8 *puts);
void	WriteChar(u8 hang,u8 lie,u8 dat);

#endif
