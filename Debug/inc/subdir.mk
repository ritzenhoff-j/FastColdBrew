################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../inc/tm_stm32f4_delay.c \
../inc/tm_stm32f4_ds18b20.c \
../inc/tm_stm32f4_gpio.c \
../inc/tm_stm32f4_hcsr04.c \
../inc/tm_stm32f4_onewire.c \
../inc/tm_stm32f4_pwm.c \
../inc/tm_stm32f4_timer_properties.c 

OBJS += \
./inc/tm_stm32f4_delay.o \
./inc/tm_stm32f4_ds18b20.o \
./inc/tm_stm32f4_gpio.o \
./inc/tm_stm32f4_hcsr04.o \
./inc/tm_stm32f4_onewire.o \
./inc/tm_stm32f4_pwm.o \
./inc/tm_stm32f4_timer_properties.o 

C_DEPS += \
./inc/tm_stm32f4_delay.d \
./inc/tm_stm32f4_ds18b20.d \
./inc/tm_stm32f4_gpio.d \
./inc/tm_stm32f4_hcsr04.d \
./inc/tm_stm32f4_onewire.d \
./inc/tm_stm32f4_pwm.d \
./inc/tm_stm32f4_timer_properties.d 


# Each subdirectory must supply rules for building sources it contributes
inc/%.o: ../inc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F411VETx -DSTM32F411E_DISCO -DDEBUG -DSTM32F411xE -DUSE_STDPERIPH_DRIVER -I"C:/Users/ritze/SystemWorkbenchWorkspace/Fast Cold Brew/StdPeriph_Driver/inc" -I"C:/Users/ritze/SystemWorkbenchWorkspace/Fast Cold Brew/inc" -I"C:/Users/ritze/SystemWorkbenchWorkspace/Fast Cold Brew/CMSIS/device" -I"C:/Users/ritze/SystemWorkbenchWorkspace/Fast Cold Brew/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


