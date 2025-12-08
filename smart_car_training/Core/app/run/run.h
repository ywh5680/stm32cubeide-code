#ifndef RUN_H
#define RUN_H

#include "main.h"
#include "motor.h"
#include "gray.h"

void Run_Init(void);
void Run_LineFollowStep(void);
void Run_SetBaseSpeed(int16_t speed);

#endif
