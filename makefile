#######################################################################################################################################################
#                                                                                                                                                     #
#                                                                                                                                                     #
#                                                                      # Directories #                                                                #
#                                                                                                                                                     #
#                                                                                                                                                     #
#######################################################################################################################################################

# root directory of all files
ROOT_DIR = /home/luis/personalDocs/embedded

# project directory (GitHub repository)
PROJECT_DIR = $(ROOT_DIR)/pomogotchi/code

# build dir
BUILD_DIR = $(PROJECT_DIR)/build

# CMSIS header Files (Cortex cmx header files and stm32f429 header files)
CMSIS_CORE = $(ROOT_DIR)/CMSIS_6/CMSIS/Core# core_cm4.h
CMSIS_DEVICE = $(ROOT_DIR)/CMSIS_6/CMSIS/Device/cmsis_device_f4# stm32f429xx.h

# Project directories
OBJ_DIR = $(BUILD_DIR)/obj
UTIL_DIR = $(PROJECT_DIR)/util# openocd config file, linker script, syscalls, startup file, debug script
BSP_DIR = $(PROJECT_DIR)/bsp# bsp files (e-ink display)
USR_DIR = $(PROJECT_DIR)/user
DRIVERS_DIR = $(PROJECT_DIR)/drivers

# build directories
BIN_DIR = $(BUILD_DIR)/bin# to store the .elf file
DEBUG_DIR = $(BUILD_DIR)/debug#to store some files
MAP_DIR = $(BUILD_DIR)/map



#######################################################################################################################################################
#                                                                                                                                                     #
#                                                                                                                                                     #
#                                                                      # Files #                                                                      #  
#                                                                                                                                                     #
#                                                                                                                                                     #
#######################################################################################################################################################

# Get names of all source files
DRIVERS_SOURCES = $(wildcard $(DRIVERS_DIR)/Src/*.c)
BSP_SOURCES = $(wildcard $(BSP_DIR)/Src/*.c)
USR_SOURCES = $(wildcard $(USR_DIR)/Src/*.c)
UTIL_SOURCES = $(wildcard $(UTIL_DIR)/*.c)

# channge .c extension for .o for object names
DRIVERS_NAMES = $(DRIVERS_SOURCES:.c=.o)
BSP_NAMES = $(BSP_SOURCES:.c=.o)
USR_NAMES = $(USR_SOURCES:.c=.o)
UTIL_NAMES = $(UTIL_SOURCES:.c=.o)

# Substitute the directory by the object's destination directory
DRIVERS_OBJS = $(patsubst $(DRIVERS_DIR)/Src/%.o, $(OBJ_DIR)/drivers/%.o, $(DRIVERS_NAMES))
BSP_OBJS = $(patsubst $(BSP_DIR)/Src/%.o, $(OBJ_DIR)/bsp/%.o, $(BSP_NAMES))
USR_OBJS = $(patsubst $(USR_DIR)/Src/%.o, $(OBJ_DIR)/usr/%.o, $(USR_NAMES))
UTIL_OBJS = $(patsubst $(UTIL_DIR)/%.o, $(OBJ_DIR)/util/%.o, $(UTIL_NAMES))

# Target file
TARGET = $(BIN_DIR)/executable.elf

# Degub files
GDB_LOG = $(DEBUG_DIR)/gdb.log
MAP_FILE = $(MAP_DIR)/project.map

#######################################################################################################################################################
#                                                                                                                                                     #
#                                                                                                                                                     #
#                                                                      # Toolchain #                                                                  #
#                                                                                                                                                     #
#                                                                                                                                                     #
#######################################################################################################################################################

CC = arm-none-eabi-gcc
MACH = cortex-m4
INC = -I $(CMSIS_CORE)/Include \
			-I $(CMSIS_DEVICE)/Include \
			-I $(BSP_DIR)/Inc \
			-I $(USR_DIR)/Inc \
			-I $(DRIVERS_DIR)/Inc \
			-I $(UTIL_DIR)

# Definition for using stm32f429xx.h
DEFINE_SYMBOLS = -D STM32F429xx

# Compiler and Linker flags
CFLAGS = -c -mcpu=$(MACH) $(INC) $(DEFINE_SYMBOLS) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -Werror -g3
LDFLAGS = -mcpu=$(MACH) $(INC) $(DEFINE_SYMBOLS) -mthumb -mfloat-abi=soft --specs=nano.specs -T $(UTIL_DIR)/stm32_ls.ld -Wl,-Map=$(MAP_FILE)

#######################################################################################################################################################
#                                                                                                                                                     #
#                                                                                                                                                     #
#                                                                      # OpenOCD #                                                                    #  
#                                                                                                                                                     #
#                                                                                                                                                     #
#######################################################################################################################################################

OPEN_OCD = openocd
OPEN_OCD_FLAGS = -f $(UTIL_DIR)/openocd.cfg


#######################################################################################################################################################
#                                                                                                                                                     #
#                                                                                                                                                     #
#                                                                      # gdb-multiarch #                                                              #  
#                                                                                                                                                     #
#                                                                                                                                                     #
#######################################################################################################################################################

GDB = gdb-multiarch
GDB_SCRIPT = --command=$(UTIL_DIR)/debug.gdb
GDB_DEBUG = -ex "target remote localhost:3333" -ex "lay src" -ex "b main" -ex "continue"

#######################################################################################################################################################
#                                                                                                                                                     #
#                                                                                                                                                     #
#                                                                      # Build and phony targets #                                                    #  
#                                                                                                                                                     #
#                                                                                                                                                     #
#######################################################################################################################################################

.PHONY: all clean load debug gdb_debug gdb_log

all: $(TARGET)

$(TARGET): $(DRIVERS_OBJS) $(BSP_OBJS) $(UTIL_OBJS) $(USR_OBJS)
	@mkdir -p $(MAP_DIR) $(DEBUG_DIR)# map and debug directories
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^


$(OBJ_DIR)/drivers/%.o: $(DRIVERS_DIR)/Src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/bsp/%.o: $(BSP_DIR)/Src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/util/%.o: $(UTIL_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^


$(OBJ_DIR)/usr/%.o: $(USR_DIR)/Src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

# load the executable into the board using openOCD
load: $(TARGET)
	$(OPEN_OCD) $(OPEN_OCD_FLAGS) -c "init; reset halt; flash write_image erase $<; verify_image $<; reset; shutdown"

# load the executable into the board and halt it (waiting for a gdb connection to be established)
debug: $(TARGET)
	$(OPEN_OCD) $(OPEN_OCD_FLAGS) -c "init; reset halt; flash write_image erase $<; verify_image $<; reset halt"

# launch gdb and set it on the main function
gdb_debug:
	$(GDB) $(TARGET) $(GDB_DEBUG)

# launch the defined gdb script and copy the content in the log file
gdb_log:
	> $(GDB_LOG) && $(GDB) $(GDB_SCRIPT)

# clean the project
clean:
	rm -rf $(OBJ_DIR)

#######################################################################################################################################################
#                                                                                                                                                     #
#                                                                                                                                                     #
#                                                                      # Files #                                                                      #  
#                                                                                                                                                     #
#                                                                                                                                                     #
#######################################################################################################################################################
