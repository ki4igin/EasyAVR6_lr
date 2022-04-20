###############################################################################
# Main application file name
###############################################################################
TARGET = $(notdir $(shell pwd))


###############################################################################
# Build path
###############################################################################
ifeq ($(DEBUG), 1)
BUILD_DIR := build/debug
else
BUILD_DIR := build/release
endif


###############################################################################
# Source
###############################################################################
# Defines
C_DEFINES := __AVR_ATmega16__
C_DEFINES += F_CPU=8000000UL

# C includes
INCLUDES:= $(shell find code -type d)

# ASM sources
A_SOURCES := $(wildcard $(addsuffix /*.s, $(INCLUDES)))

# C sources
C_SOURCES := $(wildcard $(addsuffix /*.c, $(INCLUDES)))


###############################################################################
# Compilers and Utilities binaries
###############################################################################
PREFIX = avr-
# The gcc compiler bin path (default: current folder)
ifndef GCC_PATH
GCC_PATH:= C:/toolchain/avr-gcc-9.2.0-x64-mingw/bin
endif
AS 		:= $(GCC_PATH)/$(PREFIX)gcc -xassembler-with-cpp
CC 		:= $(GCC_PATH)/$(PREFIX)gcc.exe
OBJCOPY := $(GCC_PATH)/$(PREFIX)objcopy.exe
OBJDUMP := $(GCC_PATH)/$(PREFIX)objdump.exe
SZ 		:= $(GCC_PATH)/$(PREFIX)size.exe


###############################################################################
# CFLAGS
###############################################################################
# CPU
CPU := -mmcu=atmega16

# MCU
MCU := $(CPU)

# Optimization and Debug level
ifeq ($(DEBUG), 1)
OPT := -O1 -g2
else
OPT := -Os
endif

# Compile gcc flags
FLAGS := $(OPT) $(MCU)
FLAGS += -ffunction-sections
FLAGS += -fdata-sections
FLAGS += -fpack-struct
FLAGS += -fshort-enums
FLAGS += -funsigned-char
FLAGS += -mshort-calls
FLAGS += -std=gnu11
FLAGS += -Winline -Wall -Wextra #-Wpedantic #-Werror 

AFLAGS := $(FLAGS)

CFLAGS := $(FLAGS)
CFLAGS += -MMD -MP
CFLAGS += $(addprefix -D, $(C_DEFINES)) $(addprefix -I, $(INCLUDES)) 


###############################################################################
# LFLAGS
###############################################################################
# Linking options for avr-gcc
LFLAGS := $(MCU) -Wl,--gc-sections


###############################################################################
# HFLAGS
###############################################################################
# Options for HEX file generation
HFLAGS := -j .text -j .data -O ihex


###############################################################################
# build the application
###############################################################################
# list of objects
OBJECTS := $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(A_SOURCES:.s=.o)))
vpath %.c $(sort $(INCLUDES))
vpath %.s $(sort $(INCLUDES))

# Build all
all: build
	@echo
	$(SZ) $(BUILD_DIR)/$(TARGET).elf
	@echo
	@echo -e "\033[32;1;40m""Listing:""\033[0m" $(BUILD_DIR)/$(TARGET).lss

build: info $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).lss
	
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile	
	$(CC) $(OBJECTS) $(LFLAGS) -o $@	
	
$(BUILD_DIR)/%.lss: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(OBJDUMP) -S $< > $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(OBJCOPY) $(HFLAGS) $< $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo $<
	$(CC) -c $(CFLAGS) $< -o $@	

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo $<
	$(AS) -c $(AFLAGS) $< -o $@
	 
$(BUILD_DIR):
	mkdir -p $@

###############################################################################
# clean up
###############################################################################
.PHONY: clean
clean:
	@echo -rm -fR $(dir $(BUILD_DIR))
	-rm -fR $(dir $(BUILD_DIR))

###############################################################################
# Flash
###############################################################################
.PHONY: flash
flash : 	
	$(AVRDUDE) $(DUDEFLAGS)

###############################################################################
# Info
###############################################################################
info:
	@echo -e "\033[32;1;40m""Compiler version:""\033[0m"
	$(CC) --version
	@echo -e "\033[32;1;40m""Compile flags:""\033[0m" $(FLAGS)
	@echo -e "\033[32;1;40m""Linking flags:""\033[0m" $(LFLAGS)
	@echo
	# @echo $(OBJECTS)
	# @echo 

###############################################################################
# dependencies
###############################################################################
-include $(wildcard $(BUILD_DIR)/*.d)
###############################################################################