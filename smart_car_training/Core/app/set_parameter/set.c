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
extern int16_t hrun_weight_multiplier;

/**
 * @brief 手动解析浮点数字符串
 * @param str 字符串
 * @return 浮点数值
 */
static float parse_float(const char* str)
{
    float result = 0.0f;
    float fraction = 0.0f;
    int divisor = 1;
    int sign = 1;
    int in_fraction = 0;
    
    // 跳过前导空格
    while (*str == ' ' || *str == '\t') {
        str++;
    }
    
    // 处理符号
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    
    // 解析数字
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            if (in_fraction) {
                fraction = fraction * 10.0f + (*str - '0');
                divisor *= 10;
            } else {
                result = result * 10.0f + (*str - '0');
            }
        } else if (*str == '.' && !in_fraction) {
            in_fraction = 1;
        } else if (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n') {
            break;  // 遇到空白字符停止
        } else {
            break;  // 遇到其他非数字字符停止
        }
        str++;
    }
    
    return sign * (result + fraction / divisor);
}

/**
 * @brief 处理串口调参命令
 * @param cmd 命令字符串
 */
void Set_ProcessCommand(char* cmd)
{
    char response[100];
    char param[10];
    char* space_pos;
    float value;
    
    // 调试：输出接收到的原始命令
    sprintf(response, "DEBUG: Received cmd='%s' len=%d\r\n", cmd, (int)strlen(cmd));
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
        value = parse_float(value_str);
        
        // 调试：输出解析结果
        sprintf(response, "DEBUG: param='%s' value_str='%s' value=%.2f\r\n", param, value_str, value);
        usart_send_string(response);
        
        // 设置Kp
        if (strcmp(param, "KP") == 0) {
            hrun_kp = value;
            sprintf(response, "OK: Kp=%.1f\r\n", hrun_kp);
            usart_send_string(response);
        }
        // 设置Kd
        else if (strcmp(param, "KD") == 0) {
            hrun_kd = value;
            sprintf(response, "OK: Kd=%.1f\r\n", hrun_kd);
            usart_send_string(response);
        }
        // 设置Kg
        else if (strcmp(param, "KG") == 0) {
            hrun_kg = value;
            sprintf(response, "OK: Kg=%.2f\r\n", hrun_kg);
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
        sprintf(response, "Kp:%d Kd:%d Kg:%d Spd:%d PM:%d\r\n",
                (int)hrun_kp, (int)hrun_kd, (int)(hrun_kg * 10), hrun_base_speed, hrun_weight_multiplier);
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
