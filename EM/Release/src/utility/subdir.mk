################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/utility/DiGraph.cpp \
../src/utility/DisjointSets.cpp \
../src/utility/GlobalDataStructures.cpp \
../src/utility/InputReader.cpp \
../src/utility/graph.cpp \
../src/utility/utilityFunction.cpp 

OBJS += \
./src/utility/DiGraph.o \
./src/utility/DisjointSets.o \
./src/utility/GlobalDataStructures.o \
./src/utility/InputReader.o \
./src/utility/graph.o \
./src/utility/utilityFunction.o 

CPP_DEPS += \
./src/utility/DiGraph.d \
./src/utility/DisjointSets.d \
./src/utility/GlobalDataStructures.d \
./src/utility/InputReader.d \
./src/utility/graph.d \
./src/utility/utilityFunction.d 


# Each subdirectory must supply rules for building sources it contributes
src/utility/%.o: ../src/utility/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


