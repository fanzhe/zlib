################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/minDFS/Trans_GSPANGRAPH_GRAPH.cpp 

OBJS += \
./src/minDFS/Trans_GSPANGRAPH_GRAPH.o 

CPP_DEPS += \
./src/minDFS/Trans_GSPANGRAPH_GRAPH.d 


# Each subdirectory must supply rules for building sources it contributes
src/minDFS/%.o: ../src/minDFS/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


