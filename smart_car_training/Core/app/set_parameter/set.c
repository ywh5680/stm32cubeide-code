#include "set.h"
#include "hrun.h"
#include "usart.h"
#include "float.h"
#include <stdio.h>
#include <string.h>

// 外部参数引用（需要在hrun.c中暴露）
extern float hrun_kp;
extern float hrun_kd;
extern float hrun_kg;
extern int16_t hrun_base_speed;
extern int16_t hrun_weight_multiplier;



/**
 * @brief 处理串口调参命令
 * @param cmd 命令字符串
 */
void Set_ProcessCommand(char* cmd)
{
    char response[100];
    char param[10];
    char value_str_buf[20];
    char* space_pos;
    float value;
    
    // 调试：显示接收到的原始命令
    sprintf(response, "RX: '%s'\r\n", cmd);
    usart_send_string(response);
    
    // 去除字符串末尾的回车换行符
    int len = strlen(cmd);
    while (len > 0 && (cmd[len-1] == '\r' || cmd[len-1] == '\n' || cmd[len-1] == ' ')) {
        cmd[len-1] = '\0';
        len--;
    }
    
    // 查找空格分隔符
    space_pos = strchr(cmd, ' ');
    
    if (space_pos != NULL) {
        // 提取参数名
        int param_len = space_pos - cmd;
        if (param_len >= sizeof(param)) param_len = sizeof(param) - 1;
        strncpy(param, cmd, param_len);
        param[param_len] = '\0';
        
        // 跳过多个空格，找到数值部分
        char* value_str = space_pos + 1;
        while (*value_str == ' ') value_str++;
        
        // 解析数值
        value = Float_Parse(value_str);
        
        // 设置Kp
        if (strcmp(param, "KP") == 0) {
            hrun_kp = value;
            Float_ToString(hrun_kp, 1, value_str_buf);
            sprintf(response, "OK: Kp=%s\r\n", value_str_buf);
            usart_send_string(response);
        }
        // 设置Kd
        else if (strcmp(param, "KD") == 0) {
            hrun_kd = value;
            Float_ToString(hrun_kd, 1, value_str_buf);
            sprintf(response, "OK: Kd=%s\r\n", value_str_buf);
            usart_send_string(response);
        }
        // 设置Kg
        else if (strcmp(param, "KG") == 0) {
            hrun_kg = value;
            Float_ToString(hrun_kg, 2, value_str_buf);
            sprintf(response, "OK: Kg=%s\r\n", value_str_buf);
            usart_send_string(response);
        }
        // 设置速度
        else if (strcmp(param, "SPD") == 0) {
            hrun_base_speed = (int16_t)value;
            sprintf(response, "OK: Spd=%d\r\n", hrun_base_speed);
            usart_send_string(response);
        }
        // 设置权重倍数
        else if (strcmp(param, "PM") == 0) {
            hrun_weight_multiplier = (int16_t)value;
            sprintf(response, "OK: PM=%d (weights: %d,%d,%d,%d,%d,%d,%d,%d)\r\n", 
                    hrun_weight_multiplier,
                    -4*hrun_weight_multiplier, -3*hrun_weight_multiplier, 
                    -2*hrun_weight_multiplier, -1*hrun_weight_multiplier,
                     1*hrun_weight_multiplier,  2*hrun_weight_multiplier,
                     3*hrun_weight_multiplier,  4*hrun_weight_multiplier);
            usart_send_string(response);
        }
        else {
            sprintf(response, "ERROR: Unknown param '%s'\r\n", param);
            usart_send_string(response);
        }
    }
    // GET命令 - 查看所有参数
    else if (strncmp(cmd, "GET", 3) == 0) {
        char kp_str[20], kd_str[20], kg_str[20];
        Float_ToString(hrun_kp, 1, kp_str);
        Float_ToString(hrun_kd, 1, kd_str);
        Float_ToString(hrun_kg, 2, kg_str);
        sprintf(response, "Kp:%s Kd:%s Kg:%s Spd:%d PM:%d\r\n",
                kp_str, kd_str, kg_str, hrun_base_speed, hrun_weight_multiplier);
        usart_send_string(response);
    }
    // HELP命令 - 显示帮助信息
    else if (strncmp(cmd, "HELP", 4) == 0) {
        usart_send_string("=== Commands ===\r\n");
        usart_send_string("KP <val>  - Set Kp\r\n");
        usart_send_string("KD <val>  - Set Kd\r\n");
        usart_send_string("KG <val>  - Set Kg\r\n");
        usart_send_string("SPD <val> - Set Speed\r\n");
        usart_send_string("PM <val>  - Set weight multiplier\r\n");
        usart_send_string("GET       - View params\r\n");
        usart_send_string("HELP      - Show this\r\n");
    }
    else {
        usart_send_string("ERROR: Invalid cmd\r\n");
    }
}
