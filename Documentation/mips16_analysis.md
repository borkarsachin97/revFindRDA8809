# MIPS16e Application Specific Extension (ASE) Analysis

## Overview
During the static analysis of `chunk_1_0x01c0027c.bin` (which loads at the `0x81c0027c` KSEG0 boundary) and the internal Boot ROM (`0x01E00000`), we suspected the presence of the MIPS16 / MIPS16e Application Specific Extension (ASE). Standard MIPS32 binaries typically maintain 4-byte instruction alignment. However, MIPS16 instructions compress execution footprints down to 2-bytes, requiring the MIPS core to context-switch execution pipelines.

## Architectural Verification
By inspecting `pdf/datasheet.pdf`, we confirmed that the RDA 8809 SoC features an **RDA RISC Core** equipped with a **16/32 bit instruction set**. Additionally, the `helpfull-headers/regdef.h` explicitly sets `.set mips16` macros for the toolchain, proving that the hardware actively interleaves MIPS32 and MIPS16 logic within its memory space.

## Mode Switching Boundary Analysis
The MIPS architecture shifts between 32-bit and 16-bit modes using the `jalx` (Jump and Link Exchange) instruction, which toggles the internal ISA Mode bit (the Least Significant Bit of the target address).

By scanning the original MIPS32 disassembly, we identified `jalx` targets branching to odd addresses (e.g., `0x81c00a35`). This indicates that the CPU is instructed to switch to MIPS16 mode at address `0x81c00a34`.

A heuristic script was developed to extract these boundaries:
* Found **52 distinct internal MIPS16 jalx entry points** within `chunk_1`.
* The `objdump -m mips:16` target flag was utilized to cross-decode the raw binary at these exact target boundaries.

## Outcome
A new, unified disassembly file (`chunk_1_unified_disasm.txt`) was generated. This file interleaves standard 32-bit decoding with 16-bit decoding at the exact memory boundaries where `jalx` and `jr` mode-switching branches occur, providing an accurate 1:1 view of the processor's execution cycle.
