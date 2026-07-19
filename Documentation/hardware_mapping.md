# Hardware Base Mapping for RDA 8809

Using the `helpfull-headers` provided with the source tree, we have mapped the memory regions used in the disassembly.

## Known Base Addresses
- **REG_INT_SRAM_BASE**: `0x01C00000` (Loaded as `0x81c00000` via KSEG0 mapping in MIPS to bypass cache/MMU appropriately).
  - Memory at `0x81c00000` onwards is internal SRAM. The bootloader chunks are placed here (chunk 0 at `0x81c000a0`, chunk 1 at `0x81c0027c`).
- **SYS_CTRL_BASE**: `0x01A00000`
  - System control registers for resetting hardware blocks, enabling clocks, configuring PLL, etc.
- **REG_CS0_BASE**: Boot Flash Programmed Pattern Address is mapped here for executing code off external SPI Flash or similar memory.

## Specific Addresses Used in Boot
- `0x81C000CC`: A configuration WORD loaded immediately at boot time. Used with an `andi v0, v1, 0x40` check.
- `0x81C00278`: Contains state or control variables checked right after stack initialization (`andi a0, v0, 0x3`).
- `0x81C00274`: Another control variable used to decide if the execution jumps to `__LAB_01c003c4`.
- Stack is set up inside SRAM: `addiu sp, sp, -0x28` down from whatever `sp` was set to by the ROM (which would be top of SRAM or a dedicated area).
