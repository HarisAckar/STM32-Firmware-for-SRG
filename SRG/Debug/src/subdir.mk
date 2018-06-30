################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/LED.c \
../src/SRG_Input.c \
../src/SRG_Timers.c \
../src/SRG_USART.c \
../src/delay.c \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/LED.o \
./src/SRG_Input.o \
./src/SRG_Timers.o \
./src/SRG_USART.o \
./src/delay.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/LED.d \
./src/SRG_Input.d \
./src/SRG_Timers.d \
./src/SRG_USART.d \
./src/delay.d \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"/home/haris/Dropbox/Private/SRG-SciP/STM32F4 Firmware for SRG/STM32-Firmware-for-SRG/SRG/StdPeriph_Driver/inc" -I"/home/haris/Dropbox/Private/SRG-SciP/STM32F4 Firmware for SRG/STM32-Firmware-for-SRG/SRG/inc" -I"/home/haris/Dropbox/Private/SRG-SciP/STM32F4 Firmware for SRG/STM32-Firmware-for-SRG/SRG/CMSIS/device" -I"/home/haris/Dropbox/Private/SRG-SciP/STM32F4 Firmware for SRG/STM32-Firmware-for-SRG/SRG/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


