################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../1-src/ad7606.c \
../1-src/can_config.c \
../1-src/can_data_deal.c \
../1-src/diagnosis.c \
../1-src/led_app.c \
../1-src/net_transfer.c \
../1-src/sd_save.c \
../1-src/sec_app.c \
../1-src/tfds-clb.c \
../1-src/tz_save.c \
../1-src/uart_app.c \
../1-src/update.c \
../1-src/user_data.c \
../1-src/watchdog.c 

CPP_SRCS += \
../1-src/ploygon.cpp 

OBJS += \
./1-src/ad7606.o \
./1-src/can_config.o \
./1-src/can_data_deal.o \
./1-src/diagnosis.o \
./1-src/led_app.o \
./1-src/net_transfer.o \
./1-src/ploygon.o \
./1-src/sd_save.o \
./1-src/sec_app.o \
./1-src/tfds-clb.o \
./1-src/tz_save.o \
./1-src/uart_app.o \
./1-src/update.o \
./1-src/user_data.o \
./1-src/watchdog.o 

C_DEPS += \
./1-src/ad7606.d \
./1-src/can_config.d \
./1-src/can_data_deal.d \
./1-src/diagnosis.d \
./1-src/led_app.d \
./1-src/net_transfer.d \
./1-src/sd_save.d \
./1-src/sec_app.d \
./1-src/tfds-clb.d \
./1-src/tz_save.d \
./1-src/uart_app.d \
./1-src/update.d \
./1-src/user_data.d \
./1-src/watchdog.d 

CPP_DEPS += \
./1-src/ploygon.d 


# Each subdirectory must supply rules for building sources it contributes
1-src/%.o: ../1-src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I/home/workspace/LH-APP/LH-TFDS-A/TFDS-CLB/2-inc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

1-src/%.o: ../1-src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/home/workspace/LH-APP/LH-TFDS-A/TFDS-CLB/2-inc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


