################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"/home/haris/Dropbox/Private/SRG-SciP/STM32 Firmware for SRG/SRG/StdPeriph_Driver/inc" -I"/home/haris/Dropbox/Private/SRG-SciP/STM32 Firmware for SRG/SRG/inc" -I"/home/haris/Dropbox/Private/SRG-SciP/STM32 Firmware for SRG/SRG/CMSIS/device" -I"/home/haris/Dropbox/Private/SRG-SciP/STM32 Firmware for SRG/SRG/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


