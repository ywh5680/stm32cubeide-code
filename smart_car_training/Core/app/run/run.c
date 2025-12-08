#include "run.h"

static int16_t run_base_speed = 400;
static float run_kp = 80.0f;
static float run_kd = 50.0f;
static float run_last_error = 0.0f;

static int16_t run_clamp_speed(int16_t speed)
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

void Run_Init(void)
{
    Car_Move(0, 0);
}

void Run_SetBaseSpeed(int16_t speed)
{
    run_base_speed = speed;
}

void Run_LineFollowStep(void)
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

    float error = run_last_error;
    if (count > 0)
    {
        error = (float)sum / (float)count;
    }

    float derivative = error - run_last_error;
    run_last_error = error;

    float control = run_kp * error + run_kd * derivative;
    int16_t turn = (int16_t)control;

    int16_t left_speed = run_clamp_speed(run_base_speed - turn);
    int16_t right_speed = run_clamp_speed(run_base_speed + turn);

    Car_Move(-left_speed, -right_speed);
}

