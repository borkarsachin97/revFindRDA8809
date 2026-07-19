# ====================================================================
# CUSTOM FREESTANDING MIPS TOOLCHAIN CONFIGURATION
# ====================================================================

COMPILER_PATH = /home/vixxkigoli/applications/cooltools/gcc/bin/
TOOLCHAIN_LIB_PATH = /home/vixxkigoli/applications/cooltools/gcc/lib

export LD_LIBRARY_PATH := $(TOOLCHAIN_LIB_PATH):$(LD_LIBRARY_PATH)

CC      = $(COMPILER_PATH)mips-elf-gcc
LD      = $(COMPILER_PATH)mips-elf-ld
OBJCOPY = $(COMPILER_PATH)mips-elf-objcopy
OBJDUMP = $(COMPILER_PATH)mips-elf-objdump

# [PREPROCESSOR MACROS] Add your configuration flags here
HW_FLAGS =

# Strict hardware profile compilation flags
CFLAGS  = -c -O0 -mips32 -mel -G0 -ffreestanding -nostdlib -std=c99 $(HW_FLAGS)
LDFLAGS = -EL -T link.ld -e bl_entry

# Host system compiler for the packaging tool
HOST_CC = gcc

# Build Targets
TARGET     = custom_test_payload.bin
ELF_TARGET = custom_test_payload.elf

# ====================================================================
# [OPTION] STATE MULTIPLE .C FILES HERE
# ====================================================================
SRCS       = main.c
OBJECTS    = $(SRCS:.c=.o)

PACKER_SRC  = firmWrap.c
PACKER_BIN  = ./firmWrap
FP_OUTPUT   = firm.fp

# ====================================================================
# BUILD RULES
# ====================================================================

all: $(TARGET) $(PACKER_BIN) pack disasm flash

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(ELF_TARGET)
	$(OBJCOPY) -O binary $(ELF_TARGET) $(TARGET)
	@echo "===================================================="
	@echo "[+] Raw MIPS binary payload created: $(TARGET)"
	@echo "===================================================="

# The compiler will now explicitly inherit $(HW_FLAGS) via $(CFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(PACKER_BIN): $(PACKER_SRC)
	$(HOST_CC) -O2 $(PACKER_SRC) -o $(PACKER_BIN)
	@echo "[+] Host packager utility compiled: $(PACKER_BIN)"
	@echo "===================================================="

pack: $(TARGET) $(PACKER_BIN)
	$(PACKER_BIN) $(TARGET) $(FP_OUTPUT)
	@echo "[+] Firmware container successfully packaged: $(FP_OUTPUT)"
	@echo "===================================================="

disasm: $(TARGET)
	$(OBJDUMP) -D -b binary -m mips:isa32 -EL --adjust-vma=0x81c00280 $(TARGET) > disassembly.txt
	@echo "[+] Assembly successfully generated inside disassembly.txt"
	@echo "===================================================="

flash:
	sudo ./rudra -t $(FP_OUTPUT)

clean:
	rm -f *.o $(TARGET) $(ELF_TARGET) $(PACKER_BIN) $(FP_OUTPUT) disassembly.txt
	@echo "Clean complete."

.PHONY: all clean disasm pack flash
