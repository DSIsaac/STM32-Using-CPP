################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Hardware/TIMER/TIMER.cpp 

OBJS += \
./Hardware/TIMER/TIMER.o 

CPP_DEPS += \
./Hardware/TIMER/TIMER.d 


# Each subdirectory must supply rules for building sources it contributes
Hardware/TIMER/%.o: ../Hardware/TIMER/%.cpp Hardware/TIMER/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/HASEE/Desktop/STM32/MINE/STM32 Using CPP/Hardware/TIMER" -I"C:/Users/HASEE/Desktop/STM32/MINE/STM32 Using CPP/Algorithm/Task_Scheduler" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

