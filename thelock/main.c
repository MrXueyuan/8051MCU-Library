#include "LCD1602.h"
#include "delay.h"
#include "config.h"
#include "Keyboard.h"

#include "STC12.h"

char key_number = 0;
unsigned char xdata password [6] = {'0','0','5','4','0','3'};
unsigned char data enter_psw [6] = {' ',' ',' ',' ',' ',' '};
unsigned char data input_flag = 0;
unsigned char output_num;
bit shift_flag = 0;

//把数值转换为字符
void num_judge () {
	switch (key_number) {
		case 0 : output_num = '0';
			break;
		case 1 : output_num = '1';
			break;
		case 2 : output_num = '2';
			break;
		case 3 : output_num = '3';
			break;
		case 4 : output_num = '4';
			break;
		case 5 : output_num = '5';
			break;
		case 6 : output_num = '6';
			break;
		case 7 : output_num = '7';
			break;
		case 8 : output_num = '8';
			break;
		case 9 : output_num = '9';
			break;
		default:
			output_num = '?';
	}
}

//输入值加入数组
void Arr_Refresh () {
	if (output_num != '?') {
		enter_psw [input_flag] = output_num;
	}
	else if (input_flag != 0){
		input_flag -= 1;
	}
	else if (input_flag == 0)
		input_flag == 6;
}

//刷新LCD
void LCD_Refresh () {
	PutString	(1,1,"Enter password: ");	//输出字符串 (提示)
	PutString	(2,1,&enter_psw);			//输出字符串 (密码)
	ClearBit	(2,7);						//防止溢出显示
}

int main (void) {
	delay_ms (5);					//上电延时

	Initialize_LCD();				//初始化
	ClearLine(1);					//清屏>1行
	ClearLine(2);					//清屏>2行
	PutString(1,1,"Fuck Bug!");		//输出
	
	while (1) {
		key_number = read_key ();				//读键盘输入
		if (key_number != -1) {					//判断是否输入
			PutString (1,1,"Enter password:");	//输出字符串 (提示)
			num_judge ();						//字符转换
			while (read_key () == -1) {			//读键盘按键松开
				delay_5us ();					//按键防抖
				if (read_key () == -1) {		//再次读键盘按键松开
					shift_flag = 1;				//变化标识 > 刷新LCD
					break;
				} 
				continue;
			}
		}

		if (shift_flag == 1) {		//判断变化标识
			if (input_flag == 0) {	//判断输入位标识
				ClearLine (2);		//清屏
			}
			Arr_Refresh ();			//刷新数组
			LCD_Refresh ();			//刷新LCD
			input_flag += 1;		//输入位标识移位；

			if (input_flag ==6) {	//输入位标识防溢出
				input_flag = 0;
			}
			shift_flag = 0;			//复位变化标识
		}

		// if (input_flag == 0) {
		// 	if ((input_flag == 0)&&(*enter_psw == *password)) {
		// 		ClearLine(1);
		// 		PutString (1,3,"Yes,Unlock !");
		// 		ClearLine(2);
		// 		P14 = 1;
		// 	} 
		// }
	}
}
