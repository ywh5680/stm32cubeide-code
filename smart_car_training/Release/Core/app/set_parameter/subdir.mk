################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/app/set_parameter/set.c 

OBJS += \
./Core/app/set_parameter/set.o 

C_DEPS += \
./Core/app/set_parameter/set.d 


# Each subdirectory must supply rules for building sources it contributes
Core/app/set_parameter/%.o Core/app/set_parameter/%.su Core/app/set_parameter/%.cyclo: ../Core/app/set_parameter/%.c Core/app/set_parameter/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-app-2f-set_parameter

clean-Core-2f-app-2f-set_parameter:
	-$(RM) ./Core/app/set_parameter/set.cyclo ./Core/app/set_parameter/set.d ./Core/app/set_parameter/set.o ./Core/app/set_parameter/set.su

.PHONY: clean-Core-2f-app-2f-set_parameter

