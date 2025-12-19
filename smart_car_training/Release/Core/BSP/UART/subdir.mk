################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/UART/usart.c 

OBJS += \
./Core/BSP/UART/usart.o 

C_DEPS += \
./Core/BSP/UART/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/UART/%.o Core/BSP/UART/%.su Core/BSP/UART/%.cyclo: ../Core/BSP/UART/%.c Core/BSP/UART/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-UART

clean-Core-2f-BSP-2f-UART:
	-$(RM) ./Core/BSP/UART/usart.cyclo ./Core/BSP/UART/usart.d ./Core/BSP/UART/usart.o ./Core/BSP/UART/usart.su

.PHONY: clean-Core-2f-BSP-2f-UART

