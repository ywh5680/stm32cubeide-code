################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/KEY/key.c 

OBJS += \
./Core/BSP/KEY/key.o 

C_DEPS += \
./Core/BSP/KEY/key.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/KEY/%.o Core/BSP/KEY/%.su Core/BSP/KEY/%.cyclo: ../Core/BSP/KEY/%.c Core/BSP/KEY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-KEY

clean-Core-2f-BSP-2f-KEY:
	-$(RM) ./Core/BSP/KEY/key.cyclo ./Core/BSP/KEY/key.d ./Core/BSP/KEY/key.o ./Core/BSP/KEY/key.su

.PHONY: clean-Core-2f-BSP-2f-KEY

