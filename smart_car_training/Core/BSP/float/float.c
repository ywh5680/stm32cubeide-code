#include "float.h"
#include <stdio.h>

/**
 * @brief 将字符串解析为浮点数
 */
float Float_Parse(const char* str)
{
    float result = 0.0f;
    float fraction = 0.0f;
    float divisor = 1.0f;
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
                fraction = fraction * 10.0f + (float)(*str - '0');
                divisor *= 10.0f;
            } else {
                result = result * 10.0f + (float)(*str - '0');
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
    
    return (float)sign * (result + fraction / divisor);
}

/**
 * @brief 将浮点数格式化为字符串
 */
void Float_ToString(float value, uint8_t decimal_places, char* buffer)
{
    // 处理负数
    if (value < 0) {
        *buffer++ = '-';
        value = -value;
    }
    
    // 四舍五入
    float rounding = 0.5f;
    for (uint8_t i = 0; i < decimal_places; i++) {
        rounding /= 10.0f;
    }
    value += rounding;
    
    // 提取整数部分
    int32_t int_part = (int32_t)value;
    float frac_part = value - (float)int_part;
    
    // 格式化整数部分
    sprintf(buffer, "%d", (int)int_part);
    
    // 移动到字符串末尾
    while (*buffer) buffer++;
    
    // 格式化小数部分
    if (decimal_places > 0) {
        *buffer++ = '.';
        
        for (uint8_t i = 0; i < decimal_places; i++) {
            frac_part *= 10.0f;
            int digit = (int)frac_part;
            *buffer++ = '0' + digit;
            frac_part -= (float)digit;
        }
        
        *buffer = '\0';
    }
}

/**
 * @brief 将整数解析为浮点数（带小数位数）
 */
float Float_FromInt(int32_t int_value, uint8_t decimal_places)
{
    float divisor = 1.0f;
    for (uint8_t i = 0; i < decimal_places; i++) {
        divisor *= 10.0f;
    }
    return (float)int_value / divisor;
}

/**
 * @brief 将浮点数转换为整数（带小数位数）
 */
int32_t Float_ToInt(float value, uint8_t decimal_places)
{
    float multiplier = 1.0f;
    for (uint8_t i = 0; i < decimal_places; i++) {
        multiplier *= 10.0f;
    }
    return (int32_t)(value * multiplier + 0.5f);  // 四舍五入
}

/**
 * @brief 限幅函数
 */
float Float_Clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

/**
 * @brief 线性映射
 */
float Float_Map(float value, float in_min, float in_max, float out_min, float out_max)
{
    // 防止除零
    if (in_max == in_min) return out_min;
    
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief 浮点数比较（考虑精度误差）
 */
uint8_t Float_Equal(float a, float b, float epsilon)
{
    float diff = a - b;
    if (diff < 0) diff = -diff;
    return (diff < epsilon) ? 1 : 0;
}

/**
 * @brief 获取浮点数的绝对值
 */
float Float_Abs(float value)
{
    return (value < 0) ? -value : value;
}
