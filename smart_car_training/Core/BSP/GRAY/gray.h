#ifndef GRAY_H
#define GRAY_H

#include "main.h"

// 灰度传感器GPIO定义
#define GRAY1_GPIO_PORT    GPIOA
#define GRAY1_GPIO_PIN     GPIO_PIN_5

#define GRAY2_GPIO_PORT    GPIOA
#define GRAY2_GPIO_PIN     GPIO_PIN_4

#define GRAY3_GPIO_PORT    GPIOA
#define GRAY3_GPIO_PIN     GPIO_PIN_3

#define GRAY4_GPIO_PORT    GPIOA
#define GRAY4_GPIO_PIN     GPIO_PIN_2

#define GRAY5_GPIO_PORT    GPIOA
#define GRAY5_GPIO_PIN     GPIO_PIN_1

#define GRAY6_GPIO_PORT    GPIOA
#define GRAY6_GPIO_PIN     GPIO_PIN_0

#define GRAY7_GPIO_PORT    GPIOC
#define GRAY7_GPIO_PIN     GPIO_PIN_15

#define GRAY8_GPIO_PORT    GPIOC
#define GRAY8_GPIO_PIN     GPIO_PIN_14

// 灰度传感器编号枚举
typedef enum
{
    GRAY1 = 0,
    GRAY2 = 1,
    GRAY3 = 2,
    GRAY4 = 3,
    GRAY5 = 4,
    GRAY6 = 5,
    GRAY7 = 6,
    GRAY8 = 7
} GRAY_TypeDef;

// 函数声明
void GRAY_Init(void);                          // 初始化灰度传感器GPIO
uint8_t GRAY_Read(GRAY_TypeDef sensor);        // 读取单个传感器值 (0或1)
void GRAY_ReadAll(uint8_t *values);            // 读取所有8个传感器值到数组
uint8_t GRAY_ReadByte(void);                   // 读取所有传感器值并组合成一个字节

#endif // GRAY_H
