#include "motor.h"

// PWM定时器句柄（外部定义）
extern TIM_HandleTypeDef htim3;

// 电机A引脚定义 (左电机) - 双PWM控制
// AIN1: PA6 (TIM3_CH1), AIN2: PA7 (TIM3_CH2)
#define MOTOR_A_PWM1_CHANNEL  TIM_CHANNEL_1  // PA6 - TIM3_CH1
#define MOTOR_A_PWM2_CHANNEL  TIM_CHANNEL_2  // PA7 - TIM3_CH2

// 电机B引脚定义 (右电机) - 双PWM控制  
// BIN1: PB0 (TIM3_CH3), BIN2: PB1 (TIM3_CH4)
#define MOTOR_B_PWM1_CHANNEL  TIM_CHANNEL_3  // PB0 - TIM3_CH3
#define MOTOR_B_PWM2_CHANNEL  TIM_CHANNEL_4  // PB1 - TIM3_CH4

/**
 * @brief 电机驱动初始化
 */
void Motor_Init(void)
{
    // PWM已经由CubeMX初始化，只需要启动所有PWM通道
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);  // PA6 - 电机A的AIN1
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);  // PA7 - 电机A的AIN2  
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);  // PB0 - 电机B的BIN1
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);  // PB1 - 电机B的BIN2
    
    // 初始状态：所有PWM输出为0（电机停止）
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}

/**
 * @brief 设置电机A速度和方向
 * @param speed: -1000到1000，正值前进，负值后退，0停止
 */
void Motor_A_SetSpeed(int16_t speed)
{
    if (speed > 1000) speed = 1000;
    if (speed < -1000) speed = -1000;
    
    if (speed > 0) {
        // 前进方向: AIN1=PWM, AIN2=0
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_A_PWM1_CHANNEL, speed);  // AIN1输出PWM
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_A_PWM2_CHANNEL, 0);      // AIN2输出0（接地）
    } else if (speed < 0) {
        // 后退方向: AIN1=0, AIN2=PWM  
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_A_PWM1_CHANNEL, 0);      // AIN1输出0（接地）
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_A_PWM2_CHANNEL, -speed); // AIN2输出PWM
    } else {
        // 停止: AIN1=0, AIN2=0
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_A_PWM1_CHANNEL, 0);      // AIN1=0
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_A_PWM2_CHANNEL, 0);      // AIN2=0
    }
}

/**
 * @brief 设置电机B速度和方向
 * @param speed: -1000到1000，正值前进，负值后退，0停止
 */
void Motor_B_SetSpeed(int16_t speed)
{
    if (speed > 1000) speed = 1000;
    if (speed < -1000) speed = -1000;
    
    if (speed > 0) {
        // 前进方向: BIN1=PWM, BIN2=0
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_B_PWM1_CHANNEL, speed);  // BIN1输出PWM
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_B_PWM2_CHANNEL, 0);      // BIN2输出0（接地）
    } else if (speed < 0) {
        // 后退方向: BIN1=0, BIN2=PWM
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_B_PWM1_CHANNEL, 0);      // BIN1输出0（接地）
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_B_PWM2_CHANNEL, -speed); // BIN2输出PWM
    } else {
        // 停止: BIN1=0, BIN2=0
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_B_PWM1_CHANNEL, 0);      // BIN1=0
        __HAL_TIM_SET_COMPARE(&htim3, MOTOR_B_PWM2_CHANNEL, 0);      // BIN2=0
    }
}

/**
 * @brief 直接控制左右轮速度
 * @param left_speed: 左轮速度 (-1000到1000)
 * @param right_speed: 右轮速度 (-1000到1000)
 */
void Car_Move(int16_t left_speed, int16_t right_speed)
{
    Motor_A_SetSpeed(left_speed);
    Motor_B_SetSpeed(right_speed);
}

