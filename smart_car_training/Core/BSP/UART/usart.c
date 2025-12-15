#include "usart.h"
#include "stm32f1xx_hal.h"
#include <string.h>

// UART接收缓冲区
uint8_t USART_RX_BUF[USART_REC_LEN];
volatile uint16_t USART_RX_STA = 0;
volatile uint16_t usart_rx_count = 0;

// 单字节接收变量
static uint8_t rx_byte;
static UART_HandleTypeDef *g_huart = NULL;

/**
 * @brief  初始化UART（兼容HAL库）
 * @param  huart: UART句柄指针
 * @retval None
 */
void usart_init(UART_HandleTypeDef *huart)
{
    g_huart = huart;
    USART_RX_STA = 0;
    usart_rx_count = 0;
    memset((void*)USART_RX_BUF, 0, USART_REC_LEN);
    
    // 启动中断接收
    HAL_UART_Receive_IT(huart, &rx_byte, 1);
}

/**
 * @brief  发送字符串
 * @param  str: 字符串指针
 * @retval None
 */
void usart_send_string(const char *str)
{
    if(g_huart != NULL) {
        HAL_UART_Transmit(g_huart, (uint8_t*)str, strlen(str), 1000);
    }
}

/**
 * @brief  发送数据
 * @param  data: 数据指针
 * @param  len: 数据长度
 * @retval None
 */
void usart_send_data(uint8_t *data, uint16_t len)
{
    if(g_huart != NULL) {
        HAL_UART_Transmit(g_huart, data, len, 1000);
    }
}

/**
 * @brief  获取接收到的数据长度
 * @retval 接收到的字节数
 */
uint16_t usart_get_rx_length(void)
{
    return (USART_RX_STA & 0x3FFF);  // 低14位为数据长度
}

/**
 * @brief  检查是否接收完成
 * @retval 1-接收完成, 0-未完成
 */
uint8_t usart_rx_complete(void)
{
    return (USART_RX_STA & 0x8000) ? 1 : 0;  // bit15为完成标志
}

/**
 * @brief  清空接收缓冲区
 * @retval None
 */
void usart_clear_rx_buffer(void)
{
    USART_RX_STA = 0;
    usart_rx_count = 0;
    memset((void*)USART_RX_BUF, 0, USART_REC_LEN);
}

/**
 * @brief  UART接收完成回调函数（由HAL_UART_IRQHandler调用）
 * @param  huart: UART句柄指针
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == g_huart) {
        usart_rx_count++;
        
        // 回显接收到的字符（调试用）
        // HAL_UART_Transmit(huart, &rx_byte, 1, 10);
        
        // 如果还没接收完成
        if((USART_RX_STA & 0x8000) == 0) {
            // 接收到0x0d（回车）或 0x0a（换行）
            if(rx_byte == 0x0d || rx_byte == 0x0a) {
                // 如果已经有数据，标记接收完成
                uint16_t len = USART_RX_STA & 0x3FFF;
                if(len > 0) {
                    USART_RX_STA |= 0x8000;  // 标记接收完成
                }
                // 否则忽略空的回车换行
            }
            // 普通数据
            else {
                uint16_t len = USART_RX_STA & 0x3FFF;
                if(len < USART_REC_LEN) {
                    USART_RX_BUF[len] = rx_byte;
                    USART_RX_STA = (USART_RX_STA & 0xC000) | (len + 1);
                }
                else {
                    USART_RX_STA |= 0x8000;  // 缓冲区满，标记完成
                }
            }
        }
        
        // 继续接收下一个字节
        HAL_UART_Receive_IT(huart, &rx_byte, 1);
    }
}

// 支持printf重定向（可选）
#ifdef __GNUC__
int _write(int file, char *ptr, int len)
{
    if(g_huart != NULL) {
        HAL_UART_Transmit(g_huart, (uint8_t*)ptr, len, 1000);
    }
    return len;
}
#endif	

