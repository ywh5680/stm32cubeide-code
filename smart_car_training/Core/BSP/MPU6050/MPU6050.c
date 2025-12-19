#include "main.h"
#include "SoftIIC.h"
#include "MPU6050.h"
#include "MPU6050_Reg.h"
#include <math.h>

#define MPU6050_ADDRESS		0xD0

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

// 连续读取多个寄存器（利用MPU6050自动地址递增特性）
void MPU6050_ReadMultiReg(uint8_t RegAddress, uint8_t *Data, uint8_t Length)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	
	for (uint8_t i = 0; i < Length; i++)
	{
		Data[i] = MyI2C_ReceiveByte();
		if (i == Length - 1)
		{
			MyI2C_SendAck(1);  // 最后一个字节发送NACK
		}
		else
		{
			MyI2C_SendAck(0);  // 其他字节发送ACK
		}
	}
	
	MyI2C_Stop();
}

void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t Data[14];
	
	// 从ACCEL_XOUT_H开始连续读取14字节（加速度6字节+温度2字节+陀螺仪6字节）
	MPU6050_ReadMultiReg(MPU6050_ACCEL_XOUT_H, Data, 14);
	
	*AccX = (Data[0] << 8) | Data[1];
	*AccY = (Data[2] << 8) | Data[3];
	*AccZ = (Data[4] << 8) | Data[5];
	// Data[6], Data[7] 是温度数据，这里不用
	*GyroX = (Data[8] << 8) | Data[9];
	*GyroY = (Data[10] << 8) | Data[11];
	*GyroZ = (Data[12] << 8) | Data[13];
}

void IMU_GetData(float *AccX, float *AccY, float *AccZ, 
						float *GyroX, float *GyroY, float *GyroZ)
{
	int16_t Acc_Origin[3] = {0}, Gyro_Origin[3] = {0};
	
	MPU6050_GetData(Acc_Origin, Acc_Origin + 1, Acc_Origin + 2, Gyro_Origin, Gyro_Origin + 1, Gyro_Origin + 2);
	
	*AccX = Acc_Origin[0] * 16 / 32768.0f;
	*AccY = Acc_Origin[1] * 16 / 32768.0f;
	*AccZ = Acc_Origin[2] * 16 / 32768.0f;
	
	*GyroX = Gyro_Origin[0] * 2000 / 32768.0;
	*GyroY = Gyro_Origin[1] * 2000 / 32768.0;
	*GyroZ = Gyro_Origin[2] * 2000 / 32768.0;
}

// 获取Z轴角速度（用于寻迹小车直线保持）- 优化版，只读GyroZ
float MPU6050_GetYawRate(void)
{
	uint8_t Data[2];
	int16_t GyroZ_Raw;
	
	// 只读取GyroZ的2个寄存器（0x47-0x48）
	MPU6050_ReadMultiReg(MPU6050_GYRO_ZOUT_H, Data, 2);
	GyroZ_Raw = (Data[0] << 8) | Data[1];
	
	// 转换为角速度(°/s)，量程±2000°/s
	return GyroZ_Raw * 2000.0f / 32768.0f;
}
