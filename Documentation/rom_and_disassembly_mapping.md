# Rom Mapping and Function Analysis

## 0x81E0XXXX Calls
The disassembly reveals numerous calls to `0x81E0XXXX` (`jal` and `jalx`).
```assembly
81c003bc:	0c7800b0 	jal	0x81e002c0
81c003e4:	7478040c 	jalx	0x81e01031
81c01178:	74780b21 	jalx	0x81e02c85
```
According to `mem_bridge_asm.h`, the Internal ROM is located at `REG_INT_ROM_BASE` (`0x01E00000`). In MIPS, `0x81E00000` is the `KSEG0` (uncached) mapping of this ROM.

This implies that the SRAM-loaded bootloader (`8809_00400000.fp` running from `0x81c0xxxx`) relies heavily on built-in factory ROM routines.
These functions in ROM likely include:
1. **Low-level Hardware Primitives**: Functions for initializing standard timers or doing PLL lock loops.
2. **Standard Library / Utility Functions**: Some of these might be `printf` (via UART or USB CDC), `memcpy`, or CRC routines provided by the factory ROM to save space in the flash payload.
3. **Hardware-Specific Crypto/Security Verification**: Many secure boot SOCs put the hash verification logic in ROM. The calls to `jalx 0x81e01031` occur right after evaluating flags, suggesting this could be a ROM-based verification or state-transition function.

## Mapping Source to Disassembly

### System Control (SYS_CTRL) Loops
There are many writes to `0xA1A0XXXX`, mapping to `SYS_CTRL`. For instance, clearing the system reset or configuring clocks.
A common loop pattern in the disassembly is reading a peripheral status register (like checking if the PLL is locked) and spinning until a specific bit goes high or low.

### `printf` and `sxs_fprintf`
While there is no explicit `printf` symbol in the raw disassembly, the ROM calls (`jalx 0x81e0...`) might be standard HAL debug functions. `platform/chip/hal/src/hal_debug.c` implements `sxs_vprintf`, which takes a string format, showing that formatted output is heavily used. In production bootloaders, debug traces are often routed through the `DEBUG_PORT` (`0x01A0D000` / `0xA1A0D000`) or UART (`CFG_REGS_CLK_UART`).

## High Level Logic in Source Code
The `src/flash_programmer/flash_programmer.c` file implements `main()`. It processes an array of commands mapped to uncached memory (`HAL_SYS_GET_UNCACHED_ADDR`).
The main loop checks for various `FPC_COMMAND_TYPE_T` values (like erasing sectors, writing data via `hal_SpiFlashWrite`, verifying CRCs, etc.).
This corresponds to the structure of the bootloader: after initial setup in assembly (which we analyzed in `boot_sequence.md`), it eventually transitions to a C `main()` loop that receives commands (often via USB or UART CDC) to execute Flash Programming logic.
