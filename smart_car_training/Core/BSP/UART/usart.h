#ifndef __USART_H
#define __USART_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

// 定义最大接收字节数
#define USART_REC_LEN  200

// 外部变量声明
extern uint8_t USART_RX_BUF[USART_REC_LEN];
extern volatile uint16_t USART_RX_STA;
extern volatile uint16_t usart_rx_count;

// 函数声明
void usart_init(UART_HandleTypeDef *huart);
void usart_send_string(const char *str);
void usart_send_data(uint8_t *data, uint16_t len);
uint16_t usart_get_rx_length(void);
uint8_t usart_rx_complete(void);
void usart_clear_rx_buffer(void);

#endif /* __USART_H */


