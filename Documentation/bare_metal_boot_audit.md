# Bare Metal Boot Sequence Audit (Updated)

We have determined that `helpfull-headers/boot_fpc.S` and `platform/chip/boot/8809/src/boot_rom.S` contain the native SDK mappings for the initial setup.

1. **`__xcpu_start`**: This is the native boot vector. The code loads KSEG0 SRAM bases and evaluates boot modes.
2. **CP0 Configurations**: Registers like `Status` and `Cause` are managed using explicit masks defined in `cp0.h` (like `Status_IntMASK | Status_IEc | 0x400000`).

To successfully build a completely isolated bare-metal blinky application without OS dependencies, we must extract the core of these initialization files (removing extensive SDK logging/monitor polling dependencies) and wire them to our `main.c`.
