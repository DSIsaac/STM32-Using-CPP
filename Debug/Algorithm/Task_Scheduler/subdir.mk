################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Algorithm/Task_Scheduler/Task_Scheduler.cpp 

OBJS += \
./Algorithm/Task_Scheduler/Task_Scheduler.o 

CPP_DEPS += \
./Algorithm/Task_Scheduler/Task_Scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
Algorithm/Task_Scheduler/%.o: ../Algorithm/Task_Scheduler/%.cpp Algorithm/Task_Scheduler/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/HASEE/Desktop/STM32/MINE/STM32 Using CPP/Hardware/TIMER" -I"C:/Users/HASEE/Desktop/STM32/MINE/STM32 Using CPP/Algorithm/Task_Scheduler" -I"C:/Users/HASEE/Desktop/STM32/MINE/STM32 Using CPP/Hardware/SPI" -I"C:/Users/HASEE/Desktop/STM32/MINE/STM32 Using CPP/Hardware/USART" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

