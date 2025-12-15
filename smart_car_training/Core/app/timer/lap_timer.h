#ifndef LAP_TIMER_H
#define LAP_TIMER_H

#include "stdint.h"

// 计时器状态
typedef enum {
    LAP_TIMER_IDLE = 0,     // 空闲状态
    LAP_TIMER_RUNNING,      // 计时中
    LAP_TIMER_STOPPED       // 已停止（保存了成绩）
} LapTimerState;

// 计时器结构体
typedef struct {
    uint32_t start_time;    // 开始时间戳（ms）
    uint32_t lap_time;      // 圈速时间（ms）
    LapTimerState state;    // 当前状态
} LapTimer;

// 函数声明
void LapTimer_Init(LapTimer *timer);
void LapTimer_Start(LapTimer *timer);
void LapTimer_Stop(LapTimer *timer);
void LapTimer_Reset(LapTimer *timer);
uint32_t LapTimer_GetCurrentTime(LapTimer *timer);  // 获取当前用时（ms）
uint8_t LapTimer_IsRunning(LapTimer *timer);

#endif /* LAP_TIMER_H */
