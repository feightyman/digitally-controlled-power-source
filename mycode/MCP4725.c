#include "header.h"


#define MCP4725_ADDR_WRITE 0xC0  // 默认地址（A0接地）
#define MCP4725_ADDR_READ  0xC1

/* 写入DAC寄存器（快速模式）*/
void MCP4725_SetVoltage_Fast(uint16_t voltage) {
    voltage = voltage > 4095 ? 4095 : voltage;  // 限制到12位
    
    I2C_Start();
    I2C_SendByte(MCP4725_ADDR_WRITE);
    I2C_SendByte((uint8_t)(voltage >> 4));       // 高8位
    I2C_SendByte((uint8_t)((voltage & 0x0F) << 4)); // 低4位
    I2C_Stop();
}

/* 读取DAC值 */
uint16_t MCP4725_ReadStatus(void) {
    uint8_t data[5];
    uint16_t value = 0;
    
    I2C_Start();
    I2C_SendByte(MCP4725_ADDR_READ);
    
    data[0] = I2C_ReadByte(1);  // 状态字节
    data[1] = I2C_ReadByte(1);  // 高字节
    data[2] = I2C_ReadByte(0);  // 低字节（只取高4位）
    
    I2C_Stop();
    
    value = ((data[1] << 4) | (data[2] >> 4));
    return value;
}

/* 带EEPROM存储的写入 */
void MCP4725_SetVoltage_EEPROM(uint16_t voltage) {
    voltage = voltage > 4095 ? 4095 : voltage;
    
    I2C_Start();
    I2C_SendByte(MCP4725_ADDR_WRITE);
    I2C_SendByte(0x60);  // C2=0, C1=1, C0=1 (Write DAC and EEPROM)
    I2C_SendByte((uint8_t)(voltage >> 4));
    I2C_SendByte((uint8_t)((voltage & 0x0F) << 4));
    I2C_Stop();
    
    HAL_Delay(25);  // 等待EEPROM写入完成
}

