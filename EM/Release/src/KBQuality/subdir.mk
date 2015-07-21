################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/KBQuality/GraphSimCal.cpp \
../src/KBQuality/testKBQuality.cpp 

OBJS += \
./src/KBQuality/GraphSimCal.o \
./src/KBQuality/testKBQuality.o 

CPP_DEPS += \
./src/KBQuality/GraphSimCal.d \
./src/KBQuality/testKBQuality.d 


# Each subdirectory must supply rules for building sources it contributes
src/KBQuality/%.o: ../src/KBQuality/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


