################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../basicAutomata.cpp \
../database.cpp \
../databaseGraph.cpp \
../datalogProgram.cpp \
../graphClass.cpp \
../main.cpp \
../naturalJoinFunctions.cpp \
../otherAutomata.cpp \
../parseClass.cpp \
../readingClass.cpp \
../relationClass.cpp 

OBJS += \
./basicAutomata.o \
./database.o \
./databaseGraph.o \
./datalogProgram.o \
./graphClass.o \
./main.o \
./naturalJoinFunctions.o \
./otherAutomata.o \
./parseClass.o \
./readingClass.o \
./relationClass.o 

CPP_DEPS += \
./basicAutomata.d \
./database.d \
./databaseGraph.d \
./datalogProgram.d \
./graphClass.d \
./main.d \
./naturalJoinFunctions.d \
./otherAutomata.d \
./parseClass.d \
./readingClass.d \
./relationClass.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


