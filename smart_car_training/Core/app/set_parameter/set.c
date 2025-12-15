#include "set.h"
#include "hrun.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

// 外部参数引用（需要在hrun.c中暴露）
extern float hrun_kp;
extern float hrun_kd;
extern float hrun_kg;
extern int16_t hrun_base_speed;

/**
 * @brief 处理串口调参命令
 * @param cmd 命令字符串
 */
void Set_ProcessCommand(char* cmd)
{
    char response[100];
    char param[10];
    float value;
    
    // 解析命令：参数名 + 值
    if (sscanf(cmd, "%s %f", param, &value) == 2) {
        // 设置Kp
        if (strcmp(param, "KP") == 0) {
            hrun_kp = value;
            sprintf(response, "OK: Kp=%d\r\n", (int)hrun_kp);
            usart_send_string(response);
        }
        // 设置Kd
        else if (strcmp(param, "KD") == 0) {
            hrun_kd = value;
            sprintf(response, "OK: Kd=%d\r\n", (int)hrun_kd);
            usart_send_string(response);
        }
        // 设置Kg
        else if (strcmp(param, "KG") == 0) {
            hrun_kg = value;
            sprintf(response, "OK: Kg=%d\r\n", (int)(hrun_kg * 10));
            usart_send_string(response);
        }
        // 设置速度
        else if (strcmp(param, "SPD") == 0) {
            hrun_base_speed = (int16_t)value;
            sprintf(response, "OK: Spd=%d\r\n", hrun_base_speed);
            usart_send_string(response);
        }
        else {
            usart_send_string("ERROR: Unknown param\r\n");
        }
    }
    // GET命令 - 查看所有参数
    else if (strncmp(cmd, "GET", 3) == 0) {
        sprintf(response, "Kp:%d Kd:%d Kg:%d Spd:%d\r\n",
                (int)hrun_kp, (int)hrun_kd, (int)(hrun_kg * 10), hrun_base_speed);
        usart_send_string(response);
    }
    // HELP命令 - 显示帮助信息
    else if (strncmp(cmd, "HELP", 4) == 0) {
        usart_send_string("=== Commands ===\r\n");
        usart_send_string("KP <val>  - Set Kp\r\n");
        usart_send_string("KD <val>  - Set Kd\r\n");
        usart_send_string("KG <val>  - Set Kg\r\n");
        usart_send_string("SPD <val> - Set Speed\r\n");
        usart_send_string("GET       - View params\r\n");
        usart_send_string("HELP      - Show this\r\n");
    }
    else {
        usart_send_string("ERROR: Invalid cmd\r\n");
    }
}
