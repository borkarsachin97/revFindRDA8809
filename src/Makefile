# RDA 8809 Bare-Metal Boot Makefile

# Toolchain definitions
CC = mips-elf-gcc
LD = mips-elf-ld
OBJCOPY = mips-elf-objcopy

# Compiler flags
# -mips32: Target standard 32-bit MIPS ISA
# -O2: Optimization level 2
# -G0: Disable small data sections (.sdata/.sbss) to avoid GP relative addressing issues in raw SRAM
# -nostdlib: We are bare-metal, do not link standard C libraries
CFLAGS = -mips32 -O2 -G0 -nostdlib -Wall -Wextra

# Source files
ASOURCES = bare_metal/start.S
CSOURCES = bare_metal/main.c hw_structs/sys_ctrl.c hw_structs/timer.c hw_structs/gpio.c

# Object files
AOBJECTS = $(ASOURCES:.S=.o)
COBJECTS = $(CSOURCES:.c=.o)
OBJECTS = $(AOBJECTS) $(COBJECTS)

# Output binaries
ELF = blink_payload.elf
BIN = blink_payload.bin

# Linker script
LDSCRIPT = bare_metal/link.ld

all: $(BIN)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(ELF): $(OBJECTS)
	$(LD) -T $(LDSCRIPT) $(OBJECTS) -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(ELF) $(BIN)

.PHONY: all clean
