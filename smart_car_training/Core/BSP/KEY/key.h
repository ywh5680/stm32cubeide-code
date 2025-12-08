#ifndef __KEY_H
#define __KEY_H

#include "main.h"

/* 按键引脚定义 */
#define KEY1_GPIO_PORT    GPIOB
#define KEY1_GPIO_PIN     GPIO_PIN_12

#define KEY2_GPIO_PORT    GPIOB
#define KEY2_GPIO_PIN     GPIO_PIN_13

/* 按键状态定义 */
#define KEY_PRESSED       1  // 按下时为高电平
#define KEY_RELEASED      0  // 松开时为低电平

/* 按键编号定义 */
typedef enum {
    KEY_NONE = 0,
    KEY1_PRESS = 1,
    KEY2_PRESS = 2
} KEY_Value;

/* 函数声明 */
void KEY_Init(void);
uint8_t KEY1_Read(void);
uint8_t KEY2_Read(void);
KEY_Value KEY_Scan(uint8_t mode);

#endif /* __KEY_H */
