################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/app/timer/lap_timer.c 

OBJS += \
./Core/app/timer/lap_timer.o 

C_DEPS += \
./Core/app/timer/lap_timer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/app/timer/%.o Core/app/timer/%.su Core/app/timer/%.cyclo: ../Core/app/timer/%.c Core/app/timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-app-2f-timer

clean-Core-2f-app-2f-timer:
	-$(RM) ./Core/app/timer/lap_timer.cyclo ./Core/app/timer/lap_timer.d ./Core/app/timer/lap_timer.o ./Core/app/timer/lap_timer.su

.PHONY: clean-Core-2f-app-2f-timer

