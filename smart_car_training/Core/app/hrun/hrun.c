#include "hrun.h"
#include "gray.h"
#include "MPU6050.h"
#include "motor.h"

// 可调参数（外部可访问）
int16_t hrun_base_speed = 400;
float hrun_kp = 80.0f;
float hrun_kd = 50.0f;
float hrun_kg = 0.1f;

// 内部变量
static float hrun_last_error = 0.0f;

static int16_t hrun_clamp_speed(int16_t speed)
{
    if (speed > 1000)
    {
        speed = 1000;
    }
    else if (speed < -1000)
    {
        speed = -1000;
    }
    return speed;
}

void HRun_Init(void)
{
    GRAY_Init();
    MPU6050_Init();
    Motor_Init();
    Car_Move(0, 0);
}

void HRun_SetBaseSpeed(int16_t speed)
{
    hrun_base_speed = speed;
}

void HRun_LineFollowStep(void)
{
    uint8_t sensors = GRAY_ReadByte();
    static const int8_t weights[8] = {-4, -3, -2, -1, 1, 2, 3, 4};
    int16_t sum = 0;
    int16_t count = 0;

    for (int i = 0; i < 8; i++)
    {
        if (((sensors >> i) & 0x01U) == 0U)
        {
            sum += weights[i];
            count++;
        }
    }

    float error = hrun_last_error;
    if (count > 0)
    {
        error = (float)sum / (float)count;
    }

    float derivative = error - hrun_last_error;
    hrun_last_error = error;

    // 读取陀螺仪Z轴角速度
    float gyro_z = MPU6050_GetYawRate();
    
    // PD控制 + 陀螺仪阻尼
    float control = hrun_kp * error + hrun_kd * derivative - hrun_kg * gyro_z;
    int16_t turn = (int16_t)control;

    int16_t left_speed = hrun_clamp_speed(hrun_base_speed - turn);
    int16_t right_speed = hrun_clamp_speed(hrun_base_speed + turn);

    Car_Move(left_speed, right_speed);
}

