#include "gray.h"

/**
 * @brief  初始化灰度传感器GPIO
 * @param  None
 * @retval None
 */
void GRAY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能GPIOA和GPIOC时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    // 配置GRAY1-6 GPIO引脚 (PA5-PA0)
    GPIO_InitStruct.Pin = GRAY1_GPIO_PIN | GRAY2_GPIO_PIN | GRAY3_GPIO_PIN | 
                          GRAY4_GPIO_PIN | GRAY5_GPIO_PIN | GRAY6_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;        // 输入模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;            // 无上下拉（根据硬件调整）
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 高速
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 配置GRAY7-8 GPIO引脚 (PC15, PC14)
    GPIO_InitStruct.Pin = GRAY7_GPIO_PIN | GRAY8_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;        // 输入模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;            // 无上下拉（根据硬件调整）
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 高速
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
 * @brief  读取单个灰度传感器值
 * @param  sensor: 传感器编号 (GRAY1~GRAY8)
 * @retval 传感器值: 0或1 (0-检测到黑色, 1-检测到白色，具体取决于硬件)
 */
uint8_t GRAY_Read(GRAY_TypeDef sensor)
{
    GPIO_PinState pin_state;
    
    switch(sensor)
    {
        case GRAY1:
            pin_state = HAL_GPIO_ReadPin(GRAY1_GPIO_PORT, GRAY1_GPIO_PIN);
            break;
        case GRAY2:
            pin_state = HAL_GPIO_ReadPin(GRAY2_GPIO_PORT, GRAY2_GPIO_PIN);
            break;
        case GRAY3:
            pin_state = HAL_GPIO_ReadPin(GRAY3_GPIO_PORT, GRAY3_GPIO_PIN);
            break;
        case GRAY4:
            pin_state = HAL_GPIO_ReadPin(GRAY4_GPIO_PORT, GRAY4_GPIO_PIN);
            break;
        case GRAY5:
            pin_state = HAL_GPIO_ReadPin(GRAY5_GPIO_PORT, GRAY5_GPIO_PIN);
            break;
        case GRAY6:
            pin_state = HAL_GPIO_ReadPin(GRAY6_GPIO_PORT, GRAY6_GPIO_PIN);
            break;
        case GRAY7:
            pin_state = HAL_GPIO_ReadPin(GRAY7_GPIO_PORT, GRAY7_GPIO_PIN);
            break;
        case GRAY8:
            pin_state = HAL_GPIO_ReadPin(GRAY8_GPIO_PORT, GRAY8_GPIO_PIN);
            break;
        default:
            return 0;
    }
    
    return (uint8_t)pin_state;
}

/**
 * @brief  读取所有8个灰度传感器值到数组
 * @param  values: 存储8个传感器值的数组指针 (values[0]=GRAY1, values[7]=GRAY8)
 * @retval None
 */
void GRAY_ReadAll(uint8_t *values)
{
    values[0] = GRAY_Read(GRAY1);
    values[1] = GRAY_Read(GRAY2);
    values[2] = GRAY_Read(GRAY3);
    values[3] = GRAY_Read(GRAY4);
    values[4] = GRAY_Read(GRAY5);
    values[5] = GRAY_Read(GRAY6);
    values[6] = GRAY_Read(GRAY7);
    values[7] = GRAY_Read(GRAY8);
}

/**
 * @brief  读取所有传感器值并组合成一个字节
 * @param  None
 * @retval 8个传感器值组合的字节 (bit0=GRAY1, bit7=GRAY8)
 * @note   返回值范围: 0x00-0xFF
 */
uint8_t GRAY_ReadByte(void)
{
    uint8_t result = 0;
    
    result |= (GRAY_Read(GRAY1) << 0);
    result |= (GRAY_Read(GRAY2) << 1);
    result |= (GRAY_Read(GRAY3) << 2);
    result |= (GRAY_Read(GRAY4) << 3);
    result |= (GRAY_Read(GRAY5) << 4);
    result |= (GRAY_Read(GRAY6) << 5);
    result |= (GRAY_Read(GRAY7) << 6);
    result |= (GRAY_Read(GRAY8) << 7);
    
    return result;
}
