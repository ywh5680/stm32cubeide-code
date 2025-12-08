#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"

// PWM定时器句柄声明
extern TIM_HandleTypeDef htim3;

// 电机驱动初始化
void Motor_Init(void);
// PWM电机速度控制（speed: 0-1000）
void Motor_A_SetSpeed(int16_t speed);  // 正值前进，负值后退，0停止 
void Motor_B_SetSpeed(int16_t speed);  // 正值前进，负值后退，0停止
// PWM小车运动控制（speed: 0-1000）
void Car_Move(int16_t left_speed, int16_t right_speed);  // 直接控制左右轮速度


#endif // MOTOR_H
