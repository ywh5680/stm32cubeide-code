#ifndef __FLOAT_UTILS_H
#define __FLOAT_UTILS_H

#include <stdint.h>

/**
 * @brief 将字符串解析为浮点数
 * @param str 输入字符串（支持正负号、小数点）
 * @return 解析后的浮点数
 * @note 示例: "123.45" -> 123.45, "-0.5" -> -0.5
 */
float Float_Parse(const char* str);

/**
 * @brief 将浮点数格式化为字符串
 * @param value 浮点数值
 * @param decimal_places 小数位数（0-6）
 * @param buffer 输出缓冲区（建议至少20字节）
 * @note 示例: Float_ToString(123.456, 2, buf) -> "123.46"
 */
void Float_ToString(float value, uint8_t decimal_places, char* buffer);

/**
 * @brief 将整数解析为浮点数（带小数位数）
 * @param int_value 整数值（例如：12345）
 * @param decimal_places 小数位数（例如：2表示123.45）
 * @return 浮点数
 * @note 示例: Float_FromInt(12345, 2) -> 123.45
 */
float Float_FromInt(int32_t int_value, uint8_t decimal_places);

/**
 * @brief 将浮点数转换为整数（带小数位数）
 * @param value 浮点数值
 * @param decimal_places 小数位数
 * @return 整数值
 * @note 示例: Float_ToInt(123.45, 2) -> 12345
 */
int32_t Float_ToInt(float value, uint8_t decimal_places);

/**
 * @brief 限幅函数
 * @param value 输入值
 * @param min 最小值
 * @param max 最大值
 * @return 限幅后的值
 */
float Float_Clamp(float value, float min, float max);

/**
 * @brief 线性映射
 * @param value 输入值
 * @param in_min 输入最小值
 * @param in_max 输入最大值
 * @param out_min 输出最小值
 * @param out_max 输出最大值
 * @return 映射后的值
 * @note 示例: Float_Map(50, 0, 100, 0, 1000) -> 500
 */
float Float_Map(float value, float in_min, float in_max, float out_min, float out_max);

/**
 * @brief 浮点数比较（考虑精度误差）
 * @param a 浮点数a
 * @param b 浮点数b
 * @param epsilon 误差范围（默认0.0001）
 * @return 1表示相等，0表示不等
 */
uint8_t Float_Equal(float a, float b, float epsilon);

/**
 * @brief 获取浮点数的绝对值
 * @param value 输入值
 * @return 绝对值
 */
float Float_Abs(float value);

#endif /* __FLOAT_UTILS_H */
