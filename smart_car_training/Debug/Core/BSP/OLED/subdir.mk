################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/OLED/OLED.c 

OBJS += \
./Core/BSP/OLED/OLED.o 

C_DEPS += \
./Core/BSP/OLED/OLED.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/OLED/%.o Core/BSP/OLED/%.su Core/BSP/OLED/%.cyclo: ../Core/BSP/OLED/%.c Core/BSP/OLED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/BSP/LED" -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/BSP/KEY" -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/BSP/OLED" -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/BSP/MPU6050" -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/BSP/M" -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/BSP/GRAY" -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/app/run" -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/BSP/UART" -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/app/hrun" -I"/home/h/STM32CubeIDE/workspace_1.19.0/smart_car_training/Core/app/set_parameter" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-OLED

clean-Core-2f-BSP-2f-OLED:
	-$(RM) ./Core/BSP/OLED/OLED.cyclo ./Core/BSP/OLED/OLED.d ./Core/BSP/OLED/OLED.o ./Core/BSP/OLED/OLED.su

.PHONY: clean-Core-2f-BSP-2f-OLED

