# RDA 8809 Mixed-Mode (16/32) Disassembly Analysis
**Date:** 13 July, 2026

## 1. The `init_map()` Structure Layout
A targeted structural search for the `0x1A` (26 module) loop boundary conditions inside the SRAM space reveals multiple index-based instantiation loops using MIPS16 mode.

### Fixed Pointer Assignment (`HAL_MAP_ID_COUNT = 26`)
At address `0x81c01d4c`, the payload explicitly checks `sltiu a0, 26` while iterating through the `.fixptr` linked map tables.
```assembly
    81c01d4c:   5c1a        sltiu   a0, 26
    81c01d4e:   6a05        li      v0, 5
    81c01d50:   6006        bteqz   0x81c01d5f
    81c01d52:   b205        lw      v0, 0x81c01d64
    81c01d54:   338c        sll     v1, a0, 3       # Multiply by 8 (sizeof HAL_MAP_MODULE_T)
    81c01d56:   e271        addu    a0, v0, v1      # Add offset to base address
    81c01d58:   dca0        sw      a1, 0(a0)       # Store version pointer
    81c01d5a:   dcc1        sw      a2, 4(a0)       # Store access pointer
```
This loop accurately matches our C-struct layout where each module takes 8 bytes (`version` and `access` pointers). The base address loaded into `v0` traces back to the SRAM BSS allocation for `g_halMapTable`.

## 2. The ISPI Write / Read Execution Vector
The ISPI status register base is heavily utilized within `chunk_1_unified_disasm.txt`. A recurrent polling pattern appears when writing to the ISPI (SPI) interface.

### Hardware Master Pointer Optimization
The code continuously assigns `t5` to the `0xA1A00000` system control space base, allowing peripheral accesses via offsets.
For ISPI, it explicitly utilizes the `8192` (0x2000) offset:
```assembly
    81c00b24:   a1a02000    sb      zero, 8192(t5)   # Write 0 to 0xA1A02000 (ISPI block)
```

The MIPS16 logic checks the target status register utilizing an arithmetic subtraction sequence, checking if `a1` (the read pointer) subtracted by `v0` (the write pointer) yields a space greater than `a0`:
```assembly
    81c00b14:   b604        lw      a2, 0x81c00b24
    81c00b16:   9ea5        lw      a1, 20(a2)       # Load TX_SPACE / Status
    81c00b18:   e54f        subu    v1, a1, v0       # Subtract active vector
    81c00b1a:   eb83        sltu    v1, a0           # Check space boundary
    81c00b1c:   61fb        btnez   0x81c00b15       # Branch if space not ready
```
*Note on Discrepancy:* Our `pmd_dependencies.h` implementation for `hal_IspiRegWrite` masks out `SPI_TX_SPACE` using a bitwise `GET_BITFIELD(hwp_ispi->status, SPI_TX_SPACE) == 0`. The hardware disassembly reveals the vendor dynamically calculates the FIFO offset bounds via subtraction rather than a hard bitmask, verifying active hardware queue constraints.

## 3. The `pmd_SetLevel` / PMU Bit Patterns for Level 7
The backlight level adjustment (Level 7) features specific bounding and shifting logic.

### Disassembly Boundary Checks for Level 7
We located multiple boundary checks in MIPS16 mode clamping the PMD configuration arrays exactly to `7`:
```assembly
    81c0398a:   5b07        sltiu   v1, 7           # Ensure level <= 7
    81c0398c:   f180 6018   bteqz   0x81c03cc1
    81c03990:   0f04        la      a3, 0x81c039a0  # Load switch-case lookup table
    81c03992:   3564        sll     a1, v1, 1       # Multiply level by 2
    81c03994:   e7b1        addu    a0, a3, a1      # Jump vector calculation
    81c03996:   8c60        lh      v1, 0(a0)       # Load halfword offset
    81c03998:   e779        addu    a2, a3, v1      # Add offset to base
    81c0399a:   ee00        jr      a2              # Jump to specific level 7 logic
    ...
    81c039a0:   0329        addiu   v1, sp, 164     # Jump Table Base Matrix
```
*Cross-Reference Discrepancy:*
The PMD logic extracted from the vendor disassembly utilizes a dynamically addressed switch-case jump table located at `0x81c039a0` instead of standard bitwise logical shifts when clamping level arrays (e.g., `lightLevelToPwm0`). Our `main.c` relies on arithmetic shifts `(value & 0xff) >> 4` or `>> 5` to compress the PWM register target. While mathematically equivalent, the vendor implementation bypasses ALU cycles by mapping level 7 directly to a static branch pointer.

## 4. The Clock Gating Flags
The Clock ID extraction for `FOURCC_TOOL` utilizes packed character hex constraints.
In a little-endian MIPS32 layout, `T=0x54`, `O=0x4F`, `O=0x4F`, `L=0x4C` becomes `0x4C4F4F54`.

While scanning the unified disassembly, the closest match to the Clock Sw Request parameters shows the `FOURCC` values being directly translated into raw integers and mapped within the dynamic system heap. Our implementation in `main.c`:
`hal_SwRequestClk(FOURCC_TOOL, HAL_CLK_RATE_104M);`
will securely trigger the internal software consumer registration tree and match the expected little-endian alignment of the MIPS hardware. The extraction analysis reveals that MIPS16 heavily relies on localized static data clusters (`.sdata`) stored near jump tables rather than loading massive constants directly into instructions.
