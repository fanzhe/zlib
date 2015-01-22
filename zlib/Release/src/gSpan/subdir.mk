################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gSpan/dfs.cpp \
../src/gSpan/graph.cpp \
../src/gSpan/gspan.cpp \
../src/gSpan/ismin.cpp \
../src/gSpan/misc.cpp 

OBJS += \
./src/gSpan/dfs.o \
./src/gSpan/graph.o \
./src/gSpan/gspan.o \
./src/gSpan/ismin.o \
./src/gSpan/misc.o 

CPP_DEPS += \
./src/gSpan/dfs.d \
./src/gSpan/graph.d \
./src/gSpan/gspan.d \
./src/gSpan/ismin.d \
./src/gSpan/misc.d 


# Each subdirectory must supply rules for building sources it contributes
src/gSpan/%.o: ../src/gSpan/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


