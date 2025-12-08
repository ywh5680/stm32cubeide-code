#include "main.h"
#include "MPU6050.h"

void MyI2C_W_SCL(uint8_t BitValue)
{
	HAL_GPIO_WritePin(IMU_GPIO_Port, IMU_SCL_Pin, (GPIO_PinState)BitValue);
	for(uint16_t i = 0; i < 10; i++);  
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	HAL_GPIO_WritePin(IMU_GPIO_Port, IMU_SDA_Pin, (GPIO_PinState)BitValue);
	for(uint16_t i = 0; i < 10; i++);  
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = HAL_GPIO_ReadPin(IMU_GPIO_Port, IMU_SDA_Pin);
	for(uint16_t i = 0; i < 10; i++);  
	return BitValue;
}

void MyI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	// HAL库中时钟在MX_GPIO_Init()中已使能，这里只配置GPIO
	GPIO_InitStructure.Pin = IMU_SCL_Pin | IMU_SDA_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;  // 开漏输出
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(IMU_GPIO_Port, &GPIO_InitStructure);
	
	// SCL和SDA初始化为高电平
	HAL_GPIO_WritePin(IMU_GPIO_Port, IMU_SCL_Pin | IMU_SDA_Pin, GPIO_PIN_SET);
}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
