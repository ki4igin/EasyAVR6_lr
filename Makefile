###############################################################################
# Settings
###############################################################################
ifndef GCC_PATH
GCC_PATH:= C:/toolchain/avr8/avr8-gnu-toolchain/bin
endif
PROJECTS := counter timers uart uart_buf test lcd uart_328p axe sem_fsm rtc
PROJECTS += uart_fifo


###############################################################################
# Build progects
###############################################################################
all: $(PROJECTS)

$(PROJECTS): check_gcc
	@echo -e $(COLOR_YELLOW)"Compile \"$@\""$(COLOR_NC)
	cd $@ && "$(MAKE)" all GCC_PATH=$(GCC_PATH) DEBUG=$(DEBUG)

###############################################################################
# Clean progects
###############################################################################
PROJECTS_CLEAN := $(foreach prj, $(PROJECTS), $(prj)_clean)	

.PHONY: clean
clean: $(PROJECTS_CLEAN)
	
%_clean:
	@echo -e $(COLOR_YELLOW)"Clean \"$(patsubst %_clean,%,$@)\""$(COLOR_NC)
	cd $(patsubst %_clean,%,$@) && "$(MAKE)" clean

.PHONY: check_gcc
check_gcc:
ifeq ($(wildcard $(GCC_PATH)),)
	$(error Compiler path: $(GCC_PATH) not found)
endif	

###############################################################################
# Colors for echo -e
###############################################################################
COLOR_RED		= "\033[0;31m"
COLOR_GREEN		= "\033[0;32m"
COLOR_YELLOW	= "\033[0;33m"
COLOR_NC		= "\033[0m"
###############################################################################"

