#ifndef HRUN_H
#define HRUN_H

#include "main.h"
#include "motor.h"
#include "gray.h"
#include "MPU6050.h"

// 外部可调参数
extern int16_t hrun_base_speed;
extern float hrun_kp;
extern float hrun_kd;
extern float hrun_kg;

// 函数声明
void HRun_Init(void);
void HRun_LineFollowStep(void);
void HRun_SetBaseSpeed(int16_t speed);

#endif
