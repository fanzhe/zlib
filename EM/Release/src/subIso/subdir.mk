################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/subIso/subIso.cpp 

OBJS += \
./src/subIso/subIso.o 

CPP_DEPS += \
./src/subIso/subIso.d 


# Each subdirectory must supply rules for building sources it contributes
src/subIso/%.o: ../src/subIso/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


