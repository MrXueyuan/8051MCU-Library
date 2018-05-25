#include "MCU_Doc.h"

//iic启动信号
extern void I2C_Start () {
    SDA = 1;
    SCL = 1;
    Delay5us();
    SDA=0;
    Delay5us();
    SCL = 0;
}

//iic停止信号
extern void I2C_Stop () {
    SDA = 0;
    SCL = 1;
    Delay5us();
    SDA = 1;
    Delay5us();
}

//iic应答信号
//参数：ack (0:ACK,1:NAK)
extern bit I2C_SendACK (bit ack) {
    SDA = ack;
    SCL = 1;
    Delay5us();
    SCL = 0;
    Delay5us();
}

//向iic送1bit数据