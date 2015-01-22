################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/vflib/argedit.cc \
../src/vflib/argloader.cc \
../src/vflib/argraph.cc \
../src/vflib/error.cc \
../src/vflib/gene.cc \
../src/vflib/gene_mesh.cc \
../src/vflib/match.cc \
../src/vflib/sd_state.cc \
../src/vflib/sortnodes.cc \
../src/vflib/ull_state.cc \
../src/vflib/ull_sub_state.cc \
../src/vflib/vf2_mono_state.cc \
../src/vflib/vf2_state.cc \
../src/vflib/vf2_sub_state.cc \
../src/vflib/vf_mono_state.cc \
../src/vflib/vf_state.cc \
../src/vflib/vf_sub_state.cc \
../src/vflib/xsubgraph.cc 

OBJS += \
./src/vflib/argedit.o \
./src/vflib/argloader.o \
./src/vflib/argraph.o \
./src/vflib/error.o \
./src/vflib/gene.o \
./src/vflib/gene_mesh.o \
./src/vflib/match.o \
./src/vflib/sd_state.o \
./src/vflib/sortnodes.o \
./src/vflib/ull_state.o \
./src/vflib/ull_sub_state.o \
./src/vflib/vf2_mono_state.o \
./src/vflib/vf2_state.o \
./src/vflib/vf2_sub_state.o \
./src/vflib/vf_mono_state.o \
./src/vflib/vf_state.o \
./src/vflib/vf_sub_state.o \
./src/vflib/xsubgraph.o 

CC_DEPS += \
./src/vflib/argedit.d \
./src/vflib/argloader.d \
./src/vflib/argraph.d \
./src/vflib/error.d \
./src/vflib/gene.d \
./src/vflib/gene_mesh.d \
./src/vflib/match.d \
./src/vflib/sd_state.d \
./src/vflib/sortnodes.d \
./src/vflib/ull_state.d \
./src/vflib/ull_sub_state.d \
./src/vflib/vf2_mono_state.d \
./src/vflib/vf2_state.d \
./src/vflib/vf2_sub_state.d \
./src/vflib/vf_mono_state.d \
./src/vflib/vf_state.d \
./src/vflib/vf_sub_state.d \
./src/vflib/xsubgraph.d 


# Each subdirectory must supply rules for building sources it contributes
src/vflib/%.o: ../src/vflib/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


