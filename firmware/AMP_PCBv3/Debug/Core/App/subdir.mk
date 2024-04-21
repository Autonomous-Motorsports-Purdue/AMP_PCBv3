################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/App/LoRa.c \
../Core/App/app_statemachine.c \
../Core/App/driver_ebrake.c \
../Core/App/driver_fan.c \
../Core/App/driver_status_led.c 

OBJS += \
./Core/App/LoRa.o \
./Core/App/app_statemachine.o \
./Core/App/driver_ebrake.o \
./Core/App/driver_fan.o \
./Core/App/driver_status_led.o 

C_DEPS += \
./Core/App/LoRa.d \
./Core/App/app_statemachine.d \
./Core/App/driver_ebrake.d \
./Core/App/driver_fan.d \
./Core/App/driver_status_led.d 


# Each subdirectory must supply rules for building sources it contributes
Core/App/%.o Core/App/%.su Core/App/%.cyclo: ../Core/App/%.c Core/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/App/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-App

clean-Core-2f-App:
	-$(RM) ./Core/App/LoRa.cyclo ./Core/App/LoRa.d ./Core/App/LoRa.o ./Core/App/LoRa.su ./Core/App/app_statemachine.cyclo ./Core/App/app_statemachine.d ./Core/App/app_statemachine.o ./Core/App/app_statemachine.su ./Core/App/driver_ebrake.cyclo ./Core/App/driver_ebrake.d ./Core/App/driver_ebrake.o ./Core/App/driver_ebrake.su ./Core/App/driver_fan.cyclo ./Core/App/driver_fan.d ./Core/App/driver_fan.o ./Core/App/driver_fan.su ./Core/App/driver_status_led.cyclo ./Core/App/driver_status_led.d ./Core/App/driver_status_led.o ./Core/App/driver_status_led.su

.PHONY: clean-Core-2f-App

