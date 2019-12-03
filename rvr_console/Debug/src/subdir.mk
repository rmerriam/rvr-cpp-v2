################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DataField.cpp \
../src/HeaderField.cpp \
../src/LabelField.cpp \
../src/MainForm.cpp \
../src/NCurses.cpp \
../src/NField.cpp \
../src/NForm.cpp \
../src/PowerForm.cpp \
../src/StatusForm.cpp \
../src/SystemInfoForm.cpp \
../src/main.cpp 

OBJS += \
./src/DataField.o \
./src/HeaderField.o \
./src/LabelField.o \
./src/MainForm.o \
./src/NCurses.o \
./src/NField.o \
./src/NForm.o \
./src/PowerForm.o \
./src/StatusForm.o \
./src/SystemInfoForm.o \
./src/main.o 

CPP_DEPS += \
./src/DataField.d \
./src/HeaderField.d \
./src/LabelField.d \
./src/MainForm.d \
./src/NCurses.d \
./src/NField.d \
./src/NForm.d \
./src/PowerForm.d \
./src/StatusForm.d \
./src/SystemInfoForm.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++2a -I"/home/rmerriam/devr/rvr/rvr++" -I"/home/rmerriam/devr/rvr/rvr_console/include" -I"/home/rmerriam/devr/rvr/rvr++/include" -O0 -g3 -Wall -c -fmessage-length=0 -fconcepts -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


