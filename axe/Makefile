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
DEFINES := __AVR_ATmega16__
DEFINES += F_CPU=8000000UL

# C includes
C_INCLUDES:= $(shell find code -type d)

# C sources
C_SOURCES := $(wildcard $(addsuffix /*.c, $(C_INCLUDES)))


###############################################################################
# Compilers and Utilities binaries
###############################################################################
PREFIX = avr-
# The gcc compiler bin path (default: current folder)
ifndef GCC_PATH
GCC_PATH:= C:/toolchain/avr-gcc-9.2.0-x64-mingw/bin
endif
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
CFLAGS := $(OPT) $(MCU)
CFLAGS += -ffunction-sections -fdata-sections -fpack-struct -fshort-enums
CFLAGS += -mshort-calls
CFLAGS += -std=gnu11
CFLAGS += -Winline -Wall -Wextra #-pedantic #-Werror 
CFLAGS += -MMD -MP

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
vpath %.c $(sort $(C_INCLUDES))

# build
all: $(BUILD_DIR)/$(TARGET).elf	
	$(OBJCOPY) $(HFLAGS) $< $(BUILD_DIR)/$(TARGET).hex	
	$(OBJDUMP) -P mem-usage -S $< > $(BUILD_DIR)/$(TARGET).lss
	
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile	
	$(CC) $(OBJECTS) $(LFLAGS) -o $@	
	@echo
	$(SZ) $@
	# $(OBJDUMP) -P mem-usage $@
	
	 
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo $< " "
	$(CC) -c $< $(addprefix -I, $(C_INCLUDES)) \
	$(addprefix -D, $(DEFINES)) $(CFLAGS) -o $@	

$(BUILD_DIR): info
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
	@echo -e "\033[32;1;40m""Compile flags:""\033[0m" $(CFLAGS)
	@echo -e "\033[32;1;40m""Linking flags:""\033[0m" $(LFLAGS)
	@echo
	# @echo $(OBJECTS)
	# @echo 

###############################################################################
# dependencies
###############################################################################
-include $(wildcard $(BUILD_DIR)/*.d)
###############################################################################