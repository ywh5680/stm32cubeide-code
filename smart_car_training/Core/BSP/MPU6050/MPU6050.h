#ifndef __MPU6050_H
#define __MPU6050_H

// HAL库引脚定义
#define IMU_GPIO_Port		GPIOB
#define IMU_SCL_Pin			GPIO_PIN_10
#define IMU_SDA_Pin			GPIO_PIN_11

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_ReadMultiReg(uint8_t RegAddress, uint8_t *Data, uint8_t Length);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
void IMU_GetData(float *AccX, float *AccY, float *AccZ, 
						float *GyroX, float *GyroY, float *GyroZ);

// 寻迹小车辅助函数
float MPU6050_GetYawRate(void);  // 获取Z轴角速度

#endif
