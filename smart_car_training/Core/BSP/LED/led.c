#include "led.h"

/**
 * @brief  初始化LED GPIO
 * @param  None
 * @retval None
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能GPIOB时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    // 配置LED1 GPIO引脚 (PB14)
    GPIO_InitStruct.Pin = LED1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    // 推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;            // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;   // 低速
    HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
    
    // 配置LED2 GPIO引脚 (PB15)
    GPIO_InitStruct.Pin = LED2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    // 推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;            // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;   // 低速
    HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
    
    // 初始状态设置为关闭（高电平关闭，假设LED为低电平有效）
    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief  点亮LED
 * @param  led: LED编号 (LED1, LED2, LED_ALL)
 * @retval None
 */
void LED_On(LED_TypeDef led)
{
    switch(led)
    {
        case LED1:
            HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET);
            break;
        case LED2:
            HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_GPIO_PIN, GPIO_PIN_RESET);
            break;
        case LED_ALL:
            HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_GPIO_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

/**
 * @brief  关闭LED
 * @param  led: LED编号 (LED1, LED2, LED_ALL)
 * @retval None
 */
void LED_Off(LED_TypeDef led)
{
    switch(led)
    {
        case LED1:
            HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET);
            break;
        case LED2:
            HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_GPIO_PIN, GPIO_PIN_SET);
            break;
        case LED_ALL:
            HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_GPIO_PIN, GPIO_PIN_SET);
            break;
        default:
            break;
    }
}

/**
 * @brief  切换LED状态
 * @param  led: LED编号 (LED1, LED2, LED_ALL)
 * @retval None
 */
void LED_Toggle(LED_TypeDef led)
{
    switch(led)
    {
        case LED1:
            HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN);
            break;
        case LED2:
            HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_GPIO_PIN);
            break;
        case LED_ALL:
            HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN);
            HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_GPIO_PIN);
            break;
        default:
            break;
    }
}

/**
 * @brief  控制LED状态
 * @param  led: LED编号 (LED1, LED2, LED_ALL)
 * @param  state: LED状态 (LED_ON, LED_OFF, LED_TOGGLE)
 * @retval None
 */
void LED_Control(LED_TypeDef led, LED_StateTypeDef state)
{
    switch(state)
    {
        case LED_ON:
            LED_On(led);
            break;
        case LED_OFF:
            LED_Off(led);
            break;
        case LED_TOGGLE:
            LED_Toggle(led);
            break;
        default:
            break;
    }
}

/**
 * @brief  获取LED状态
 * @param  led: LED编号 (LED1, LED2)
 * @retval LED状态: 0-关闭, 1-点亮
 * @note   LED_ALL不支持状态读取
 */
uint8_t LED_GetState(LED_TypeDef led)
{
    GPIO_PinState pin_state;
    
    switch(led)
    {
        case LED1:
            pin_state = HAL_GPIO_ReadPin(LED1_GPIO_PORT, LED1_GPIO_PIN);
            return (pin_state == GPIO_PIN_RESET) ? 1 : 0;  // 低电平为点亮
        case LED2:
            pin_state = HAL_GPIO_ReadPin(LED2_GPIO_PORT, LED2_GPIO_PIN);
            return (pin_state == GPIO_PIN_RESET) ? 1 : 0;  // 低电平为点亮
        default:
            return 0;
    }
}