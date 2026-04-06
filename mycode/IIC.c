#include "header.h"


/* 宏定义 */
#define SCL_PIN        GPIO_PIN_4
#define SCL_PORT       GPIOB
#define SDA_PIN        GPIO_PIN_3
#define SDA_PORT       GPIOB
#define I2C_DELAY      5       // 微秒延时

/* 软件I2C基本操作 */
void I2C_Delay(void) {
    HAL_Delay(1);  // 简化延时，实际需要根据时钟调整
}

void I2C_Start(void) {
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
    I2C_Delay();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
}

void I2C_Stop(void) {
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    I2C_Delay();
}

uint8_t I2C_WaitAck(void) {
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    I2C_Delay();
    uint8_t ack = HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    return ack;
}

void I2C_SendByte(uint8_t data) {
    for(uint8_t i=0; i<8; i++) {
        HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        data <<= 1;
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
        I2C_Delay();
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    }
    I2C_WaitAck();
}


uint8_t I2C_ReadByte(uint8_t ack) {
    uint8_t data = 0;
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    for(uint8_t i=0; i<8; i++) {
        data <<= 1;
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
        I2C_Delay();
        if(HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN)) data |= 0x01;
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, ack ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    return data;
}


