# https://makefiletutorial.com/#makefile-cookbook

TARGET_EXE := lox

BUILD_DIR := bin
SRC_DIRS := src

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# List of available debug options
DEBUG_OPTIONS := DEBUG_PRINT_CODE DEBUG_TRACE_EXECUTION DEBUG_STRESS_GC DEBUG_LOG_GC

CC := gcc
CFLAGS := $(INC_FLAGS) -MMD -MP $(foreach opt,$(DEBUG_OPTIONS),$(if $($(opt)),-D$(opt)))

# The final build step.
$(BUILD_DIR)/$(TARGET_EXE): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
