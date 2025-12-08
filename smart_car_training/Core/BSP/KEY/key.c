#include "key.h"

/**
 * @brief  按键初始化
 * @param  无
 * @retval 无
 */
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能 GPIOB 和 AFIO 时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
    
    /* 禁用 JTAG，保留 SWD（释放 PB3/PB4，但不影响 PB12/PB13）*/
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    /* 配置 KEY1 (PB12) */
    GPIO_InitStruct.Pin = KEY1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;  // 内部下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);

    /* 配置 KEY2 (PB13) */
    GPIO_InitStruct.Pin = KEY2_GPIO_PIN;
    HAL_GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @brief  读取 KEY1 状态
 * @param  无
 * @retval KEY_PRESSED(1) 或 KEY_RELEASED(0)
 */
uint8_t KEY1_Read(void)
{
    return HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
}

/**
 * @brief  读取 KEY2 状态
 * @param  无
 * @retval KEY_PRESSED(1) 或 KEY_RELEASED(0)
 */
uint8_t KEY2_Read(void)
{
    return HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
}

/**
 * @brief  按键扫描函数
 * @param  mode: 0-不支持连续按, 1-支持连续按
 * @retval KEY_NONE(0), KEY1_PRESS(1), KEY2_PRESS(2)
 */
KEY_Value KEY_Scan(uint8_t mode)
{
    static uint8_t key_flag = 1;  // 按键松开标志
    
    if (mode) {
        key_flag = 1;  // 支持连续按
    }
    
    if (key_flag && (KEY1_Read() == KEY_PRESSED || KEY2_Read() == KEY_PRESSED))
    {
        HAL_Delay(10);  // 消抖延时
        key_flag = 0;
        
        if (KEY1_Read() == KEY_PRESSED) {
            return KEY1_PRESS;
        }
        else if (KEY2_Read() == KEY_PRESSED) {
            return KEY2_PRESS;
        }
    }
    else if (KEY1_Read() == KEY_RELEASED && KEY2_Read() == KEY_RELEASED)
    {
        key_flag = 1;  // 按键松开
    }
    
    return KEY_NONE;
}
