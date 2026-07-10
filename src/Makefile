# RDA 8809 Bare-Metal Boot Makefile

# Toolchain definitions
CC = mips-elf-gcc
LD = mips-elf-ld
OBJCOPY = mips-elf-objcopy

# Compiler flags
CFLAGS = -mips32 -O2 -G0 -mno-abicalls -fno-pic -nostdlib -Wall -Wextra -I../helpfull-headers

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
	$(CC) $(CFLAGS) -DCT_ASM -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(ELF) $(BIN)

.PHONY: all clean
