################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/30010_io.c \
../Src/HAL_TIMER.c \
../Src/PWM.c \
../Src/ansi.c \
../Src/ball.c \
../Src/charset.c \
../Src/joystick.c \
../Src/lcd.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f30x.c \
../Src/timer.c 

OBJS += \
./Src/30010_io.o \
./Src/HAL_TIMER.o \
./Src/PWM.o \
./Src/ansi.o \
./Src/ball.o \
./Src/charset.o \
./Src/joystick.o \
./Src/lcd.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f30x.o \
./Src/timer.o 

C_DEPS += \
./Src/30010_io.d \
./Src/HAL_TIMER.d \
./Src/PWM.d \
./Src/ansi.d \
./Src/ball.d \
./Src/charset.d \
./Src/joystick.d \
./Src/lcd.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f30x.d \
./Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F302R8Tx -DSTM32F3 -DNUCLEO_F302R8 -DDEBUG -DUSE_STDPERIPH_DRIVER=1 -c -I"C:/Users/benya/Documents/GitHub/SpaceInvaders30010/SpaceInvaders/SPL/inc" -I"C:/Users/benya/Documents/GitHub/SpaceInvaders30010/SpaceInvaders/Inc" -I"C:/Users/benya/Documents/GitHub/SpaceInvaders30010/SpaceInvaders/cmsis" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/30010_io.d ./Src/30010_io.o ./Src/HAL_TIMER.d ./Src/HAL_TIMER.o ./Src/PWM.d ./Src/PWM.o ./Src/ansi.d ./Src/ansi.o ./Src/ball.d ./Src/ball.o ./Src/charset.d ./Src/charset.o ./Src/joystick.d ./Src/joystick.o ./Src/lcd.d ./Src/lcd.o ./Src/main.d ./Src/main.o ./Src/syscalls.d ./Src/syscalls.o ./Src/sysmem.d ./Src/sysmem.o ./Src/system_stm32f30x.d ./Src/system_stm32f30x.o ./Src/timer.d ./Src/timer.o

.PHONY: clean-Src

