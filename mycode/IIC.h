#ifndef __IIC_H
#define __IIC_H


void I2C_Delay(void);

void I2C_Start(void);

void I2C_Stop(void);
uint8_t I2C_WaitAck(void);

void I2C_SendByte(uint8_t data);

uint8_t I2C_ReadByte(uint8_t ack);


#endif
