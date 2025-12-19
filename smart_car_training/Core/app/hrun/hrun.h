#ifndef HRUN_H
#define HRUN_H

#include "main.h"
#include "motor.h"
#include "gray.h"
#include "MPU6050.h"

// 寻迹模式定义
typedef enum {
    LINE_MODE_BLACK = 0,  // 黑线寻迹（检测黑色）
    LINE_MODE_WHITE = 1   // 白线寻迹（检测白色）
} LineMode_t;

// 外部可调参数
extern int16_t hrun_base_speed;
extern float hrun_kp;
extern float hrun_kd;
extern float hrun_kg;
extern int16_t hrun_weight_multiplier;
extern LineMode_t hrun_line_mode;

// 函数声明
void HRun_Init(void);
void HRun_LineFollowStep(void);
void HRun_SetBaseSpeed(int16_t speed);
void HRun_ToggleLineMode(void);
const char* HRun_GetLineModeName(void);

#endif
