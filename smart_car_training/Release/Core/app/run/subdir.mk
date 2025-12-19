################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/app/run/run.c 

OBJS += \
./Core/app/run/run.o 

C_DEPS += \
./Core/app/run/run.d 


# Each subdirectory must supply rules for building sources it contributes
Core/app/run/%.o Core/app/run/%.su Core/app/run/%.cyclo: ../Core/app/run/%.c Core/app/run/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-app-2f-run

clean-Core-2f-app-2f-run:
	-$(RM) ./Core/app/run/run.cyclo ./Core/app/run/run.d ./Core/app/run/run.o ./Core/app/run/run.su

.PHONY: clean-Core-2f-app-2f-run

