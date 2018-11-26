################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FCB_GPIO_Init.c \
../src/FCB_GPIO_Pins.c \
../src/FCB_PWM.c \
../src/FCB_PressureSensor.c \
../src/FCB_SonarSensor.c \
../src/FCB_TempSensor.c \
../src/FCB_Tests_RealTime.c \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/FCB_GPIO_Init.o \
./src/FCB_GPIO_Pins.o \
./src/FCB_PWM.o \
./src/FCB_PressureSensor.o \
./src/FCB_SonarSensor.o \
./src/FCB_TempSensor.o \
./src/FCB_Tests_RealTime.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/FCB_GPIO_Init.d \
./src/FCB_GPIO_Pins.d \
./src/FCB_PWM.d \
./src/FCB_PressureSensor.d \
./src/FCB_SonarSensor.d \
./src/FCB_TempSensor.d \
./src/FCB_Tests_RealTime.d \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F411VETx -DSTM32F411E_DISCO -DDEBUG -DSTM32F411xE -DUSE_STDPERIPH_DRIVER -I"C:/Users/ritze/SystemWorkbenchWorkspace/Fast Cold Brew/StdPeriph_Driver/inc" -I"C:/Users/ritze/SystemWorkbenchWorkspace/Fast Cold Brew/inc" -I"C:/Users/ritze/SystemWorkbenchWorkspace/Fast Cold Brew/CMSIS/device" -I"C:/Users/ritze/SystemWorkbenchWorkspace/Fast Cold Brew/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


