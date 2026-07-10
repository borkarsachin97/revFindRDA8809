# RDA 8809 Reverse Engineering Documentation

This documentation collection details the boot sequence and hardware initialization of the RDA 8809 SoC based on the reverse-engineered `8809_00400000.fp` firmware chunks.

## Sections
1. [Boot Sequence & Control Flow](boot_sequence.md)
2. [Hardware Memory Mappings](hardware_mapping.md)
3. [Peripheral Analysis](hardware_peripherals.md)
4. [SRAM & CPU Init](sram_cpu_init.md)
5. [USB & Flash Subsystems](usb_flash_subsystems.md)

## Summary of Execution
1. ROM sets up minimal hardware and maps the binary into SRAM starting at `0x01C00000`.
2. Execution jumps to `0x81C000A0` (Uncached KSEG0 mapping) which redirects to `0x81C00280`.
3. The setup reads boot profile flags (`0x81C000CC`, `0x81C00278`), configures the stack inside SRAM, and validates memory blocks.
4. Intensive interaction occurs with the `SYS_CTRL` module at `0xA1A00000` to adjust clock trees, timers, and peripheral resets.
5. Control flows into external routines (SPI Flash/ROM) starting at `0x81E0XXXX` using `jalx` which signifies jumping to 16-bit MIPS code (MIPS16e) for code density.

## Factory BootROM Integrations
6. [Rom & Source Code Mapping](rom_and_disassembly_mapping.md)
7. [Comprehensive Disassembly Functions](comprehensive_disassembly_functions.md)

Detailed documentation regarding hardware registers Q&A can be found in [Hardware Q&A](hardware_QnA.md).

## Sub-Register and Hardware Level Breakdown
8. [Register Level Analysis (Sub-registers)](register_level_analysis.md)
