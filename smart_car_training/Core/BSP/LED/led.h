#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

// LED GPIO 端口和引脚定义
#define LED1_GPIO_PORT    GPIOB
#define LED1_GPIO_PIN     GPIO_PIN_14
#define LED1_GPIO_CLK()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define LED2_GPIO_PORT    GPIOB
#define LED2_GPIO_PIN     GPIO_PIN_15
#define LED2_GPIO_CLK()   __HAL_RCC_GPIOB_CLK_ENABLE()

// LED 编号定义
typedef enum {
    LED1 = 0,
    LED2 = 1,
    LED_ALL = 2
} LED_TypeDef;

// LED 状态定义
typedef enum {
    LED_OFF = 0,
    LED_ON = 1,
    LED_TOGGLE = 2
} LED_StateTypeDef;

// 函数声明
void LED_Init(void);
void LED_On(LED_TypeDef led);
void LED_Off(LED_TypeDef led);
void LED_Toggle(LED_TypeDef led);
void LED_Control(LED_TypeDef led, LED_StateTypeDef state);
uint8_t LED_GetState(LED_TypeDef led);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */