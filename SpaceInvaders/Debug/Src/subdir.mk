################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/30010_io.c \
../Src/ansi.c \
../Src/asteroids.c \
../Src/ball.c \
../Src/bullet.c \
../Src/charset.c \
../Src/draw.c \
../Src/enemy.c \
../Src/hwdisplay.c \
../Src/joystick.c \
../Src/lcd.c \
../Src/main.c \
../Src/player.c \
../Src/powerup.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f30x.c \
../Src/timer.c 

OBJS += \
./Src/30010_io.o \
./Src/ansi.o \
./Src/asteroids.o \
./Src/ball.o \
./Src/bullet.o \
./Src/charset.o \
./Src/draw.o \
./Src/enemy.o \
./Src/hwdisplay.o \
./Src/joystick.o \
./Src/lcd.o \
./Src/main.o \
./Src/player.o \
./Src/powerup.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f30x.o \
./Src/timer.o 

C_DEPS += \
./Src/30010_io.d \
./Src/ansi.d \
./Src/asteroids.d \
./Src/ball.d \
./Src/bullet.d \
./Src/charset.d \
./Src/draw.d \
./Src/enemy.d \
./Src/hwdisplay.d \
./Src/joystick.d \
./Src/lcd.d \
./Src/main.d \
./Src/player.d \
./Src/powerup.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f30x.d \
./Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F302R8Tx -DSTM32F3 -DNUCLEO_F302R8 -DDEBUG -DUSE_STDPERIPH_DRIVER=1 -c -I"C:/Users/benya/Documents/GitHub/SpaceInvaders30010/SpaceInvaders/SPL/inc" -I"C:/Users/benya/Documents/GitHub/SpaceInvaders30010/SpaceInvaders/Inc" -I"C:/Users/benya/Documents/GitHub/SpaceInvaders30010/SpaceInvaders/cmsis" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/30010_io.d ./Src/30010_io.o ./Src/ansi.d ./Src/ansi.o ./Src/asteroids.d ./Src/asteroids.o ./Src/ball.d ./Src/ball.o ./Src/bullet.d ./Src/bullet.o ./Src/charset.d ./Src/charset.o ./Src/draw.d ./Src/draw.o ./Src/enemy.d ./Src/enemy.o ./Src/hwdisplay.d ./Src/hwdisplay.o ./Src/joystick.d ./Src/joystick.o ./Src/lcd.d ./Src/lcd.o ./Src/main.d ./Src/main.o ./Src/player.d ./Src/player.o ./Src/powerup.d ./Src/powerup.o ./Src/syscalls.d ./Src/syscalls.o ./Src/sysmem.d ./Src/sysmem.o ./Src/system_stm32f30x.d ./Src/system_stm32f30x.o ./Src/timer.d ./Src/timer.o

.PHONY: clean-Src

