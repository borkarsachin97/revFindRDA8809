# SRAM Memory Limits and Physical Partitions

As part of the structural analysis of the RDA 8809 architecture, we have mapped the explicit physical and logical partition bounds based on the hard 64 KB memory limit verified by the Boot ROM MIPS overflow routines. This serves as the foundational reference for our custom toolchain and bare-metal linker verifications.

## 📦 64 KB SRAM Architectural Boundaries & Workspace
- **Physical Memory Start**: `0x81C00000`
- **Physical Memory End (Ceiling)**: `0x81C10000`
- **Total Capacity**: Exactly 65,536 bytes (64 KB / `0x10000`)

## 🗺️ Structural Memory Map Allocations

### Vendor Header & Status Matrix (`0x81C00000` to `0x81C0027F`)
- **Size**: 640 bytes (`0x280`)
- **Content**: Power-on hardware flags, system boot status parameters seeded by the Boot ROM (e.g., clock masks at `0x81C000CC` and configuration tables at `0x81C00278`).

### Active Code Execution Segment (`0x81C00280` to `0x81C05984`)
- **Size**: 22,276 bytes (~21.75 KB)
- **Content**: The current upper bounds of our binary code footprint (`.text`, `.rodata`, and data elements).

### Available Headroom / Stack Space (`0x81C05985` to `0x81C10000`)
- **Size**: 42,620 bytes (~41.62 KB)
- **Dynamic Control**: The MIPS CPU stack pointer (`$sp`) initializes at the absolute maximum physical ceiling of `0x81C10000`. As our functions call and allocate stack frames, the stack pointer pushes downward into this region toward the lower numbers.

## 🔍 Structural Safety Assessment
Because our binary code only grows upward from the bottom to `0x81C05984`, and our execution stack pushes downward from `0x81C10000`, we have a massive 41.62 KB safety cushion remaining in the middle. The risk of a stack collision or runtime overflow crashing into our program code within this 64 KB space is practically zero.
