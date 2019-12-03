################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: rvr_console

dependents:
	-cd /home/rmerriam/devr/rvr/rvr_console/Release && $(MAKE) clean all
	-cd /home/rmerriam/devr/rvr/rvr++/Debug && $(MAKE) clean all

# Tool invocations
rvr_console: $(OBJS) $(USER_OBJS) /home/rmerriam/devr/rvr/rvr_console/Release/rvr_console /home/rmerriam/devr/rvr/rvr++/Debug/librvr++.a
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L"/home/rmerriam/devr/rvr/rvr++/Debug" -o "rvr_console" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(INO_DEPS)$(PDE_DEPS)$(CPP_DEPS)$(C_DEPS) rvr_console
	-@echo ' '

.PHONY: all clean dependents
/home/rmerriam/devr/rvr/rvr_console/Release/rvr_console:
/home/rmerriam/devr/rvr/rvr++/Debug/librvr++.a:

-include ../makefile.targets
