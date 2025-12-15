#include "lap_timer.h"
#include "main.h"  // 包含 HAL_GetTick()

/**
 * @brief 初始化计时器
 * @param timer 计时器结构体指针
 */
void LapTimer_Init(LapTimer *timer)
{
    timer->start_time = 0;
    timer->lap_time = 0;
    timer->state = LAP_TIMER_IDLE;
}

/**
 * @brief 开始计时
 * @param timer 计时器结构体指针
 */
void LapTimer_Start(LapTimer *timer)
{
    timer->start_time = HAL_GetTick();
    timer->state = LAP_TIMER_RUNNING;
}

/**
 * @brief 停止计时并保存成绩
 * @param timer 计时器结构体指针
 */
void LapTimer_Stop(LapTimer *timer)
{
    if (timer->state == LAP_TIMER_RUNNING)
    {
        timer->lap_time = HAL_GetTick() - timer->start_time;
        timer->state = LAP_TIMER_STOPPED;
    }
}

/**
 * @brief 重置计时器
 * @param timer 计时器结构体指针
 */
void LapTimer_Reset(LapTimer *timer)
{
    timer->start_time = 0;
    timer->lap_time = 0;
    timer->state = LAP_TIMER_IDLE;
}

/**
 * @brief 获取当前用时（毫秒）
 * @param timer 计时器结构体指针
 * @return 当前用时（ms）
 *         - 计时中：返回实时用时
 *         - 已停止：返回保存的圈速
 *         - 空闲：返回 0
 */
uint32_t LapTimer_GetCurrentTime(LapTimer *timer)
{
    if (timer->state == LAP_TIMER_RUNNING)
    {
        return HAL_GetTick() - timer->start_time;
    }
    else if (timer->state == LAP_TIMER_STOPPED)
    {
        return timer->lap_time;
    }
    return 0;
}

/**
 * @brief 检查计时器是否正在运行
 * @param timer 计时器结构体指针
 * @return 1=运行中，0=未运行
 */
uint8_t LapTimer_IsRunning(LapTimer *timer)
{
    return (timer->state == LAP_TIMER_RUNNING) ? 1 : 0;
}
