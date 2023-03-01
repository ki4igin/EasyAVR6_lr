###############################################################################
# Settings
###############################################################################
ifndef GCC_PATH
GCC_PATH:= C:/toolchain/avr8/avr8-gnu-toolchain/bin
endif
PROJECTS := counter timers uart uart_buf test lcd uart_328p axe sem_fsm rtc
PROJECTS += uart_fifo adc adc_fast


###############################################################################
# Build progects
###############################################################################
all: $(PROJECTS)

$(PROJECTS): check_gcc
	@echo -e $(COLOR_Y)"Compile \"$@\""$(COLOR_NC)
	cd $@ && "$(MAKE)" all GCC_PATH=$(GCC_PATH) DEBUG=$(DEBUG)

###############################################################################
# Clean progects
###############################################################################
PROJECTS_CLEAN := $(foreach prj, $(PROJECTS), $(prj)_clean)	

.PHONY: clean
clean: $(PROJECTS_CLEAN)
	
%_clean:
	@echo -e $(COLOR_Y)"Clean \"$(patsubst %_clean,%,$@)\""$(COLOR_NC)
	cd $(patsubst %_clean,%,$@) && "$(MAKE)" clean

.PHONY: check_gcc
check_gcc:
ifeq ($(wildcard $(GCC_PATH)),)
	$(error Compiler path: $(GCC_PATH) not found)
endif	


GIT_TAG := $(subst -, ,$(shell git describe --tags --long))
GIT_VER := $(word 1, $(GIT_TAG))
GIT_COMMIT := $(word 3, $(GIT_TAG))

BUILD_VER := $(GIT_VER)
BUILD_VER_NUM := $(subst ., ,$(BUILD_VER))
ifeq ($(words $(BUILD_VER_NUM)), 3)	
	BUILD_VER_MAJOR := $(word 1, $(BUILD_VER_NUM))
	BUILD_VER_MINOR := $(word 2, $(BUILD_VER_NUM))
	BUILD_VER_NUMBER := $(word 3, $(BUILD_VER_NUM))
	BUILD_VER_NUM := $$((\
		$(BUILD_VER_MAJOR) * 65536 + $(BUILD_VER_MINOR) * 256 + $(BUILD_VER_NUMBER)\
	))
else ifeq ($(words $(BUILD_VER_NUM)), 2)	
	BUILD_VER_MAJOR := $(word 1, $(BUILD_VER_NUM))
	BUILD_VER_MINOR := $(word 2, $(BUILD_VER_NUM))
	BUILD_VER_NUM := $$((\
		$(BUILD_VER_MAJOR) * 256 + $(BUILD_VER_MINOR)\
	))
else
	BUILD_VER_NUM := 0
endif

BUILD_COMMIT_NUM := $(shell echo $(GIT_COMMIT) | tr a-f A-F)
BUILD_COMMIT_NUM := $(subst g, 0x,$(BUILD_COMMIT_NUM))


# .PHONY: info
info: check_tag
	@echo $(GIT_TAG)
	@echo $(GIT_VER)
	@echo $(GIT_COMMIT)
	@echo $(BUILD_VER_NUM)
	@echo $(BUILD_COMMIT_NUM)

check_tag:
ifeq ($(GIT_TAG), )
	$(call echo_yellow,git tags not found)
endif	
ifeq ($(BUILD_VER_NUM), 0)	
	 $(call echo_yellow,format version \"$(BUILD_VER)\" not correct)
endif	

###############################################################################
# Colors for echo -e
###############################################################################
COLOR_RED		= "\033[0;31m"
COLOR_GREEN		= "\033[0;32m"
COLOR_YELLOW	= "\033[0;33m"
COLOR_NC		= "\033[0m"

###############################################################################
# Colors echo functions
###############################################################################
echo_red = @echo -e $(COLOR_RED)$(1)$(COLOR_NC)
echo_green = @echo -e $(COLOR_GREEN)$(1)$(COLOR_NC)
echo_yellow = @echo -e $(COLOR_YELLOW)$(1)$(COLOR_NC)

###############################################################################"