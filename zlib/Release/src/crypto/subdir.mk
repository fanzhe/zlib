################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/crypto/bigMatrix.cpp \
../src/crypto/cgbe.cpp 

OBJS += \
./src/crypto/bigMatrix.o \
./src/crypto/cgbe.o 

CPP_DEPS += \
./src/crypto/bigMatrix.d \
./src/crypto/cgbe.d 


# Each subdirectory must supply rules for building sources it contributes
src/crypto/%.o: ../src/crypto/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


